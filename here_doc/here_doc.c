/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:35:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 14:25:40 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			prepare_here_doc(t_tree_node *node, t_env *env);
static char	*_here_doc_fork_error_handler(t_env *env);
static int	_exec_hd_child(t_redirect *redirect, int fd, bool is_expandable,
				t_env *env);
static char	*_exec_here_doc_parent_process(char *tmpfile, int *status, int fd,
				t_env *env);
char		*here_doc_handler(t_redirect *redirect, t_env *env);

int	prepare_here_doc(t_tree_node *node, t_env *env)
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
			tmpfile = here_doc_handler(curr, env);
			if (!tmpfile)
				return (EXIT_FAILURE);
			free(curr->filename);
			curr->filename = tmpfile;
		}
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}

static char	*_here_doc_fork_error_handler(t_env *env)
{
	perror("minishell: fork:");
	env->prev_exit_status = EXIT_FAILURE;
	return (NULL);
}

static int	_exec_hd_child(t_redirect *redirect, int fd, bool is_expandable,
		t_env *env)
{
	char	*s;
	bool	end_by_delimiter;

	end_by_delimiter = false;
	setup_here_doc_signal_handlers();
	s = readline("> ");
	while (s)
	{
		if (ft_strcmp(s, redirect->filename) == 0)
		{
			free(s);
			end_by_delimiter = true;
			break ;
		}
		if (is_expandable)
			s = here_doc_expander(s, env);
		if (!s)
			return (close(fd), EXIT_FAILURE);
		ft_putendl_fd(s, fd);
		free(s);
		s = readline("> ");
	}
	if (end_by_delimiter == false)
		error_heredoc_delimited_by_eof(redirect->filename);
	return (close(fd), 0);
}

static char	*_exec_here_doc_parent_process(char *tmpfile, int *status, int fd,
		t_env *env)
{
	setup_parent_wait_signal_handlers();
	wait(status);
	setup_interactive_signal_handlers();
	if (WIFEXITED(*status) && WEXITSTATUS(*status) == EXIT_FAILURE)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		close(fd);
		env->prev_exit_status = 1;
		return (NULL);
	}
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		close(fd);
		env->prev_exit_status = -1;
		return (NULL);
	}
	close(fd);
	return (tmpfile);
}

/*Error msgs are collectly handled in remove_quotes(), shmktmpfd(),
	and _here_doc_fork_error_handler.*/
char	*here_doc_handler(t_redirect *redirect, t_env *env)
{
	int		fd;
	char	*tmpfile;
	bool	is_expandable;
	int		status;
	pid_t	pid;

	is_expandable = true;
	if (have_quotes(redirect->filename) == true)
	{
		is_expandable = false;
		if (remove_quotes(redirect) == EXIT_FAILURE)
			return (NULL);
	}
	fd = sh_mktmpfd(&tmpfile);
	if (fd == -1)
	{
		env->prev_exit_status = EXIT_FAILURE;
		return (NULL);
	}
	pid = fork();
	if (pid == -1)
		return (_here_doc_fork_error_handler(env));
	if (pid == 0)
		free_for_exit(env, _exec_hd_child(redirect, fd, is_expandable, env));
	return (_exec_here_doc_parent_process(tmpfile, &status, fd, env));
}
