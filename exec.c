/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/11 18:31:14 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
環境変数をどのタイミングで構造体にいれるか
*/

#include "minishell.h"
exec_ast();
exec_and_or();

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

long exec_cmd_lst(t_tree_node **lst, int size)
{
    int i;
    pid_t pid;
    long status;

    i = -1;
    while(++i < size)
    {
        pid = fork();
        if (pid == -1)
            break;
        if (pid == 0)
        {
            exec_command_helper(lst[i]);
            free_cmd_lst(lst);
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
    
    readline("> ");
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
            fd = here_doc_handler(curr->filename);
            if (fd == -1)
                ;
            dup2(fd, STDIN_FILENO);
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
                perror();
                return ;
            }
            dup2(fd, curr->io_number);
        }
        close(fd);
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
    tmp_path = ft_strjoin("./builtin/", cmd_node->data.command.args[0]);
    if (!tmp_path)
    {
        perror("malloc :");
        return (NULL);
    }
    execve(tmp_path, cmd_node->data.command.args, envp);
    free(tmp_path);
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

void exec_command_helper(t_tree_node *cmd_node)
{
    char *cmd_path;

    exec_redirection(cmd_node->data.command.redirects);
    if (!ft_strchr('/'))
    {
        find_builtin(cmd_node, envp);
        find_path(cmd_node, envp);
    }
    execve(args[0], args, envp);
    exit(EXIT_FAILURE);
}