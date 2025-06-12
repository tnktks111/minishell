/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/12 19:45:25 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
環境変数をどのタイミングで構造体にいれるか
*/

#include "minishell.h"

long exec_ast(t_tree_node *root);
long exec_and_or(t_tree_node *root);
long exec_pipeline(t_tree_node *root);
long exec_cmd_lst(t_tree_node **lst, int size);
int here_doc_handler(char *limiter);
void exec_redirection(t_redirect *redirect);
char **get_env_table(char *var_name);
void find_builtin(t_tree_node *cmd_node, char *envp[]);
void find_path(t_tree_node *cmd_node, char *envp[]);
long exec_command_helper(t_tree_node *cmd_node);
bool is_builtin(char *s);

long exec_ast(t_tree_node *root)
{
    t_tree_node *curr;
    long prev_exit_status;

    curr = root->left;
    while (curr->kind == NODE_AND || curr->kind == NODE_OR)
        curr = curr->left;
    exec_and_or(curr);
    curr = curr->parent;
    while ((prev_exit_status && curr->kind == NODE_AND)|| (!prev_exit_status && curr->kind == NODE_OR))
    {
        prev_exit_status = exec_and_or(curr->right);
        curr = curr->parent;
    }
    if (curr->kind == NODE_ROOT)
        return (prev_exit_status);
}

long exec_and_or(t_tree_node *root)
{
    root->data.pipeline.exit_status = exec_pipeline(root->left);
    if (root->data.pipeline.have_bang == true)
        return (!root->data.pipeline.exit_status);
    else
        return (root->data.pipeline.exit_status);
}
long exec_pipeline(t_tree_node *root)
{
    t_tree_node *curr;
    t_tree_node **cmd_list;
    int i;
    int cmd_cnt;
    
    cmd_cnt = 1;
    curr = root->left;
    while (curr->kind == NODE_PIPE)
    {
        cmd_cnt++;
        curr = curr->left;
    }
    if (cmd_cnt == 1)
        return (exec_solo_cmd(curr));
    cmd_list = (t_tree_node **)malloc(sizeof(t_tree_node *) * cmd_cnt);
    i = -1;
    while (++i < cmd_cnt)
    {
        cmd_list[i] = curr;
        if (i != 0)
            curr = curr->parent;
        curr = curr->parent->right;
    }
    return (exec_cmd_lst(cmd_list, cmd_cnt));
}

long exec_solo_cmd(t_tree_node *curr)
{
    pid_t pid;
    int status;

    if (is_builtin(curr->data.command.args[0]))
        return(exec_builtin(curr));
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork :");
            return (EXIT_FAILURE);
        }
        if (pid == 0)
        {
            exec_redirection(curr->data.command.redirects);
            if (!ft_strchr(curr->data.command.args[0], '/'))
                find_path(curr, envp);
            execve(curr->data.command.args[0], curr->data.command.args, envp);
            exit(EXIT_FAILURE);
        }
        else
            wait(&status);
        return (status);
    }
}

long exec_cmd_lst(t_tree_node **lst, int size)
{
    int i;
    pid_t pid;
    long status;
    int pipefd[2];
    int read_fd;

    read_fd = STDIN_FILENO;
    i = -1;
    while(++i < size)
    {
        if (i < size - 1 && pipe(pipefd) == -1)
            break;
        pid = fork();
        if (pid == -1)
            break;
        if (pid == 0)
        {
            if (read_fd != STDIN_FILENO)
            {
                dup2(read_fd, STDIN_FILENO);
                close(read_fd);
            }
            if (i < size - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            exec_command_helper(lst[i]);
        }
        if (read_fd != STDIN_FILENO)
            close(read_fd);
        if (i < size - 1)
        {
            close(pipefd[1]);
            read_fd = pipefd[0];
        }
    }
    waitpid(pid, &status, 0);
    i = -1;
    while(++i < size - 1)
        wait(NULL);
    return (status);
}

int here_doc_handler(char *limiter)
{
    int fd;
    char *tmpfile;
    char *s;
    int lim_len;

    fd = sh_mktmpfd(&tmpfile);
    if (fd == -1)
        return (EXIT_FAILURE);
    s = readline("> ");
    while(s)
    {
        if (ft_strncmp(s, limiter, lim_len) == 0 && s[lim_len] == 0)
        {
            free(s);
            break;
        }
        ft_putendl_fd(s, fd);
        free(s);
        s = readline("> ");
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    unlink(tmpfile);
    free(tmpfile);
    return (EXIT_SUCCESS);
}

void exec_redirection(t_redirect *redirect)
{
    t_redirect *curr;
    int fd;
    int open_flags;

    curr = redirect;
    while (curr)
    {
        if (curr->kind == REDIR_HEREDOC)
        {
            if (here_doc_handler(curr->filename) == EXIT_FAILURE)
                return ;
        }
        else
        {
            if (curr->kind == REDIR_IN)
                open_flags = O_RDONLY;
            else if (curr->kind == REDIR_APPEND)
                open_flags = O_WRONLY | O_CREAT | O_TRUNC;
            else
                open_flags = O_WRONLY | O_CREAT | O_APPEND;
            fd = open(curr->filename, open_flags, 0644);
            if (fd == -1)
            {
                perror("open :");
                return ;
            }
            dup2(fd, curr->io_number);
            close(fd);
        }
        curr = curr->next;
    }
}

char **get_env_table(char *var_name)
{
    char *envp;
    char **env_table;

    envp = getenv(var_name);
    env_table = ft_split(envp, ';');
    return (env_table);
}

void find_builtin(t_tree_node *cmd_node, char *envp[])
{
    char *tmp_path;
    long exit_status;

    tmp_path = ft_strjoin("./builtin/", cmd_node->data.command.args[0]);
    if (!tmp_path)
    {
        perror("malloc :");
        return (NULL);
    }
    exit_status = exec_builtin(cmd_node);
    free(tmp_path);
    exit(exit_status);
}

void find_path(t_tree_node *cmd_node, char *envp[])
{
    char **prefix_table;
    char *tmp_path;
    int i;

    prefix_table = get_env_table("PATH");
    i = -1;
    while(prefix_table[++i])
    {
        tmp_path = ft_strjoin(prefix_table[i], cmd_node->data.command.args[0]);
        if (!tmp_path)
        {
            perror("malloc :");
            return (NULL);
        }
        execve(tmp_path, cmd_node->data.command.args, envp);
        free(tmp_path);
    }
    free_splited_data(prefix_table);
    error_cmd_not_found();
    exit(127);
}

long exec_command_helper(t_tree_node *cmd_node)
{
    char *cmd_path;

    exec_redirection(cmd_node->data.command.redirects);
    if (!ft_strchr(cmd_node->data.command.args[0], '/'))
    {
        find_builtin(cmd_node, envp);
        find_path(cmd_node, envp);
    }
    execve(args[0], args, envp);
    exit(EXIT_FAILURE);
}

bool is_builtin(char *s)
{
    if (ft_strncmp(s, "echo", 4) == 0 && s[4] == 0)
        return (true);
    if (ft_strncmp(s, "cd", 2) == 0 && s[2] == 0)
        return (true);
    if (ft_strncmp(s, "pwd", 3) == 0 && s[3] == 0)
        return (true);
    if (ft_strncmp(s, "export", 6) == 0 && s[6] == 0)
        return (true);
    if (ft_strncmp(s, "unset", 5) == 0 && s[5] == 0)
        return (true);
    if (ft_strncmp(s, "env", 3) == 0 && s[3] == 0)
        return (true);
    if (ft_strncmp(s, "exit", 4) == 0 && s[4] == 0)
        return (true);
    return (false);
}

long exec_builtin(t_tree_node *node)
{
    if (ft_strncmp(node->data.command.args[0], "echo", 4) == 0 && node->data.command.args[0][4] == 0)
        return (builtin_echo(node));
    if (ft_strncmp(node->data.command.args[0], "cd", 2) == 0 && node->data.command.args[0][2] == 0)
        return (builtin_cd(node));
    if (ft_strncmp(node->data.command.args[0], "pwd", 3) == 0 && node->data.command.args[0][3] == 0)
        return (builtin_pwd(node));
    if (ft_strncmp(node->data.command.args[0], "export", 4) == 0 && node->data.command.args[0][4] == 0)
        return (builtin_export(node));
    if (ft_strncmp(node->data.command.args[0], "unset", 5) == 0 && node->data.command.args[0][5] == 0)
        return (builtin_unset(node));
    if (ft_strncmp(node->data.command.args[0], "env", 3) == 0 && node->data.command.args[0][3] == 0)
        return (builtin_env(node));
    if (ft_strncmp(node->data.command.args[0], "exit", 4) == 0 && node->data.command.args[0][4] == 0)
        return (builtin_exit(node));
}