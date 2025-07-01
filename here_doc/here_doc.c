/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:35:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 22:27:20 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			prepare_here_doc(t_tree_node *node, t_env *env, int *pipefd);
static char	*_here_doc_fork_error_handler(t_env *env);
static int	_exec_hd_child(t_redirect *redirect, int fd, bool is_expandable,
				t_env *env);
static char	*_exec_here_doc_parent_process(char *tmpfile, int *status, int fd,
				t_env *env);
char		*here_doc_handler(t_redirect *redirect, t_env *env, int *pipefd);

int	prepare_here_doc(t_tree_node *node, t_env *env, int *pipefd)
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
			tmpfile = here_doc_handler(curr, env, pipefd);
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

	setup_here_doc_signal_handlers();
	s = get_next_line("> ", STDIN_FILENO);
	while (s)
	{
		if (g_rcv_heredoc_sig == 1)
			break;
		if (ft_gnl_strcmp(s, redirect->filename) == 0)
			return (free(s), close(fd), 0);
		if (is_expandable)
			s = here_doc_expander(s, env);
		if (!s)
			return (close(fd), EXIT_FAILURE);
		ft_putstr_fd(s, fd);
		free(s);
		s = get_next_line("> ", STDIN_FILENO);
	}
	close(fd);
	if (g_rcv_heredoc_sig == 1)	
		return (free(s), g_rcv_heredoc_sig = 0, 130);
	ft_putchar_fd('\n', STDERR_FILENO);
	error_heredoc_delimited_by_eof(redirect->filename);
	close(fd);
	return (0);
}

static char	*_exec_here_doc_parent_process(char *tmpfile, int *status, int fd,
		t_env *env)
{
	close(fd);
	setup_parent_wait_signal_handlers();
	wait(status);
	setup_interactive_signal_handlers();
	if (WIFEXITED(*status) && WEXITSTATUS(*status) != EXIT_SUCCESS)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		unlink(tmpfile);
		free(tmpfile);
		if (WEXITSTATUS(*status) == 130)
			env->prev_exit_status = -1;
		else
			env->prev_exit_status = 1;
		return (NULL);
	}
	return (tmpfile);
}

/*Error msgs are collectly handled in remove_quotes(), shmktmpfd(),
	and _here_doc_fork_error_handler.*/
char	*here_doc_handler(t_redirect *redirect, t_env *env, int *pipefd)
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
		return (env->prev_exit_status = EXIT_FAILURE, NULL);
	pid = fork();
	if (pid == -1)
		return (close_pipefd(pipefd), _here_doc_fork_error_handler(env));
	if (pid == 0)
	{
		close_pipefd(pipefd);
		free_for_exit(env, _exec_hd_child(redirect, fd, is_expandable, env));
	}
	return (_exec_here_doc_parent_process(tmpfile, &status, fd, env));
}
