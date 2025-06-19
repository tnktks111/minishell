/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/19 16:40:47 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
環境変数をどのタイミングで構造体にいれるか
*/

#include "minishell.h"

/*astの探査*/
unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_and_or(t_tree_node *root, t_env *env);
int				exec_pipeline(t_tree_node *root, t_env *env);
/*redirection & here_doc*/
int				prepare_here_doc(t_tree_node *node);
char			*here_doc_handler(char *limiter);
void			exec_redirection(t_redirect *redirect);
void			backup_stdin_out(int *stdin_out);
void			restore_stdin_out(int *stdin_out);
/*utils*/
char			**get_path_prefix(t_env *env);
bool			is_builtin(char *s);
/*個々のコマンドの実行*/
void			find_builtin(t_tree_node *cmd_node, t_env *env);
void			find_path(t_tree_node *cmd_node, t_env *env);
unsigned char	exec_command_helper(t_tree_node *cmd_node, t_env *env);
unsigned char	exec_builtin(t_tree_node *node, t_env *env);
int				exec_solo_cmd(t_tree_node *curr, t_env *env);

unsigned char	exec_ast(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;
	int			prev_exit_status;

	curr = root->left;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
		curr = curr->left;
	prev_exit_status = exec_and_or(curr, env);
	if (prev_exit_status == -1)
	{
		env->prev_exit_status = 130;
		return (130);
	}
	curr = curr->parent;
	while ((!prev_exit_status && curr->kind == NODE_AND) || (prev_exit_status
			&& curr->kind == NODE_OR))
	{
		prev_exit_status = exec_and_or(curr->right, env);
		if (prev_exit_status == -1)
		{
			env->prev_exit_status = 130;
			return (130);
		}
		curr = curr->parent;
	}
	return (prev_exit_status);
}

int	exec_and_or(t_tree_node *root, t_env *env)
{
	env->envp = decode_table(env);
	root->data.pipeline.exit_status = exec_pipeline(root->left, env);
	if (root->data.pipeline.have_bang == true)
	{
		env->prev_exit_status = !root->data.pipeline.exit_status;
		return (!root->data.pipeline.exit_status);
	}
	else
	{
		env->prev_exit_status = root->data.pipeline.exit_status;
		return (root->data.pipeline.exit_status);
	}
}

void	setup_pipefd(t_pipefd *fd, t_tree_node *node, bool is_start)
{
	if (is_start)
	{
		if (fd->read_fd != STDIN_FILENO)
		{
			dup2(fd->read_fd, STDIN_FILENO);
			close(fd->read_fd);
		}
		if (node->parent->kind == NODE_PIPE)
		{
			close(fd->pipefd[0]);
			dup2(fd->pipefd[1], STDOUT_FILENO);
			close(fd->pipefd[1]);
		}
	}
	else
	{
		if (fd->read_fd != STDIN_FILENO)
			close(fd->read_fd);
		if (node->parent->kind == NODE_PIPE)
		{
			close(fd->pipefd[1]);
			fd->read_fd = fd->pipefd[0];
		}
	}
}

/*fork, pipeのエラーハンドリングあとで*/
int	exec_pipeline(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;
	pid_t		pid;
	size_t		cnt;
	t_pipefd	fd;
	int			status;

	curr = root;
	cnt = 0;
	fd.read_fd = STDIN_FILENO;
	if (curr->kind == NODE_SIMPLE_COMMAND)
		return (exec_solo_cmd(curr, env));
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	while (curr->kind != NODE_PIPE_LINE)
	{
		if (curr->parent->kind == NODE_PIPE && pipe(fd.pipefd) == -1)
			return (perror_string("pipe: "));
		cnt++;
		prepare_here_doc(curr);
		pid = fork();
		if (pid == -1)
			return (perror_string("fork: "));
		if (pid == 0)
		{
			setup_child_signal_handlers();
			setup_pipefd(&fd, curr, true);
			exec_command_helper(curr, env);
		}
		setup_pipefd(&fd, curr, false);
		curr = curr->parent;
	}
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	while (--cnt > 0)
		wait(NULL);
	setup_interactive_signal_handlers();
	if (WIFEXITED(status))
		return ((unsigned char)WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			return (-1);
		}
		return (128 + WTERMSIG(status));
	}
	return (EXIT_FAILURE);
}

void	backup_stdin_out(int *stdin_out)
{
	stdin_out[0] = dup(STDIN_FILENO);
	if (stdin_out[0] == -1)
	{
		perror("dup :");
		return ;
	}
	stdin_out[1] = dup(STDOUT_FILENO);
	if (stdin_out[1] == -1)
		perror("dup :");
}

void	restore_stdin_out(int *stdin_out)
{
	dup2(stdin_out[0], STDIN_FILENO);
	dup2(stdin_out[1], STDOUT_FILENO);
}

int	exec_solo_cmd(t_tree_node *curr, t_env *env)
{
	pid_t			pid;
	unsigned char	status;
	int				wait_status;
	int				stdin_out[2];

	prepare_here_doc(curr);
	if (is_builtin(curr->data.command.args[0]))
	{
		backup_stdin_out(stdin_out);
		exec_redirection(curr->data.command.redirects);
		status = exec_builtin(curr, env);
		restore_stdin_out(stdin_out);
		return (status);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror_string("fork: "));
		if (pid == 0)
		{
			setup_child_signal_handlers();
			exec_redirection(curr->data.command.redirects);
			if (!ft_strchr(curr->data.command.args[0], '/'))
				find_path(curr, env);
			execve(curr->data.command.args[0], curr->data.command.args,
				env->envp);
			exit(EXIT_FAILURE);
		}
		setup_parent_wait_signal_handlers();
		wait(&wait_status);
		setup_interactive_signal_handlers();
		if (WIFEXITED(wait_status))
		{
			return ((unsigned char)WEXITSTATUS(wait_status));
		}
		else if (WIFSIGNALED(wait_status))
		{
			if (WTERMSIG(wait_status) == SIGINT)
			{
				ft_putchar_fd('\n', STDERR_FILENO);
				return (-1);
			}
			if (WTERMSIG(wait_status) == SIGQUIT)
			{
				ft_putstr_fd("Quit", STDERR_FILENO);
				// if (WCOREDUMP(wait_status))
				// 	ft_putstr_fd(" (core dumped)", STDERR_FILENO);
			}
			return (128 + WTERMSIG(wait_status));
		}
		return (EXIT_FAILURE);
	}
}

char	*here_doc_handler(char *limiter)
{
	int		fd;
	char	*tmpfile;
	char	*s;

	fd = sh_mktmpfd(&tmpfile);
	if (fd == -1)
		return (NULL);
	s = readline("> ");
	while (s)
	{
		if (ft_strcmp(s, limiter) == 0)
		{
			free(s);
			break ;
		}
		// here_doc_expander(s);
		ft_putendl_fd(s, fd);
		free(s);
		s = readline("> ");
	}
	close(fd);
	return (tmpfile);
}

int	prepare_here_doc(t_tree_node *node)
{
	t_redirect	*curr;
	char		*tmpfile;

	if (node->right)
		curr = node->right->data.command.redirects;
	else
		curr = node->data.command.redirects;
	while (curr)
	{
		if (curr->kind == REDIR_HEREDOC)
		{
			tmpfile = here_doc_handler(curr->filename);
			if (!tmpfile)
				return (EXIT_FAILURE);
			free(curr->filename);
			curr->filename = tmpfile;
		}
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}

void	exec_redirection(t_redirect *redirect)
{
	t_redirect	*curr;
	int			fd;
	int			open_flags;

	curr = redirect;
	while (curr)
	{
		if (curr->kind == REDIR_IN)
			open_flags = O_RDONLY;
		else if (curr->kind == REDIR_APPEND)
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (curr->kind == REDIR_HEREDOC)
			open_flags = O_RDONLY;
		else
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(curr->filename, open_flags, 0644);
		if (fd == -1)
			return (perror("open :"));
		dup2(fd, curr->io_number);
		close(fd);
		curr = curr->next;
	}
}

char	**get_path_prefix(t_env *env)
{
	char	*val;
	char	**res;

	val = ft_search("PATH", env);
	if (!val)
		return (NULL);
	res = ft_split(val, ':');
	return (res);
}

void	find_builtin(t_tree_node *cmd_node, t_env *env)
{
	unsigned char	exit_status;

	if (!is_builtin(cmd_node->data.command.args[0]))
		return ;
	exit_status = exec_builtin(cmd_node, env);
	exit(exit_status);
}

void	find_path(t_tree_node *cmd_node, t_env *env)
{
	char	**prefix_table;
	char	*tmp;
	char	*tmp_path;
	int		i;

	prefix_table = get_path_prefix(env);
	i = -1;
	// printf("%s_\n", env->envp[0]);
	while (prefix_table[++i])
	{
		tmp = ft_strjoin(prefix_table[i], "/");
		tmp_path = ft_strjoin(tmp, cmd_node->data.command.args[0]);
		free(tmp);
		if (!tmp_path)
		{
			perror("malloc :");
			return ;
		}
		execve(tmp_path, cmd_node->data.command.args, env->envp);
		free(tmp_path);
	}
	free_splited_data(prefix_table);
	error_cmd_not_found(cmd_node->data.command.args[0]);
	exit(127);
}

unsigned char	exec_command_helper(t_tree_node *node, t_env *env)
{
	t_tree_node	*cmd_node;

	if (node->right)
		cmd_node = node->right;
	else
		cmd_node = node;
	exec_redirection(cmd_node->data.command.redirects);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND)
	{
		if (!ft_strchr(cmd_node->data.command.args[0], '/'))
		{
			find_builtin(cmd_node, env);
			find_path(cmd_node, env);
		}
		execve(cmd_node->data.command.args[0], cmd_node->data.command.args,
			env->envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		exit(exec_ast(cmd_node, env));
	}
}

bool	is_builtin(char *s)
{
	if (!s)
		return (false);
	if (ft_strcmp(s, "echo") == 0)
		return (true);
	if (ft_strcmp(s, "cd") == 0)
		return (true);
	if (ft_strcmp(s, "pwd") == 0)
		return (true);
	if (ft_strcmp(s, "export") == 0)
		return (true);
	if (ft_strcmp(s, "unset") == 0)
		return (true);
	if (ft_strcmp(s, "env") == 0)
		return (true);
	if (ft_strcmp(s, "exit") == 0)
		return (true);
	return (false);
}

unsigned char	exec_builtin(t_tree_node *node, t_env *env)
{
	if (ft_strcmp(node->data.command.args[0], "echo") == 0)
		return (builtin_echo(node, env));
	// if (ft_strcmp(node->data.command.args[0], "cd") == 0)
	// 	return (builtin_cd(node, env));
	if (ft_strcmp(node->data.command.args[0], "pwd") == 0)
		return (builtin_pwd(node, env));
	if (ft_strcmp(node->data.command.args[0], "export") == 0)
		return (builtin_export(node, env));
	if (ft_strcmp(node->data.command.args[0], "unset") == 0)
		return (builtin_unset(node, env));
	if (ft_strcmp(node->data.command.args[0], "env") == 0)
		return (builtin_env(node, env));
	if (ft_strcmp(node->data.command.args[0], "exit") == 0)
		return (builtin_exit(node, env));
	else
		return (EXIT_FAILURE);
}
