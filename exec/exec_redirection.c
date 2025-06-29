/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:14:27 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/29 19:13:19 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redir_flag_builder(t_redirect_kind kind);
int			exec_redirection(t_redirect *redirect);

static int	redir_flag_builder(t_redirect_kind kind)
{
	if (kind == REDIR_IN)
		return (O_RDONLY);
	else if (kind == REDIR_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (kind == REDIR_HEREDOC)
		return (O_RDONLY);
	else
		return (O_WRONLY | O_CREAT | O_TRUNC);
}

static void	save_fds(int saved_fds[3])
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	saved_fds[2] = dup(STDERR_FILENO);
}

static void	restore_fds(int saved_fds[3], bool error_occurred)
{
	if (error_occurred)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		dup2(saved_fds[1], STDOUT_FILENO);
		dup2(saved_fds[2], STDERR_FILENO);
	}
	close(saved_fds[0]);
	close(saved_fds[1]);
	close(saved_fds[2]);
}

static int	dup2error_handler(int saved_fds[3], int fd, int io_number)
{
	ft_puterr_nbr(io_number, strerror(errno));
	restore_fds(saved_fds, true);
	close(fd);
	return (EXIT_FAILURE);
}

int	exec_redirection(t_redirect *redirect)
{
	t_redirect	*curr;
	int			fd;
	int			saved_fds[3];

	save_fds(saved_fds);
	curr = redirect;
	while (curr)
	{
		fd = open(curr->filename, redir_flag_builder(curr->kind), 0644);
		if (fd == -1)
		{
			ft_puterr_general(curr->filename, strerror(errno));
			restore_fds(saved_fds, true);
			return (EXIT_FAILURE);
		}
		if (dup2(fd, curr->io_number) == -1)
			return (dup2error_handler(saved_fds, fd, curr->io_number));
		if (fd != curr->io_number)
			close(fd);
		curr = curr->next;
	}
	restore_fds(saved_fds, false);
	return (EXIT_SUCCESS);
}
