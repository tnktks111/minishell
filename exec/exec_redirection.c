/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:14:27 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 16:36:13 by ttanaka          ###   ########.fr       */
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

int	exec_redirection(t_redirect *redirect)
{
	t_redirect	*curr;
	int			fd;

	curr = redirect;
	while (curr)
	{
		fd = open(curr->filename, redir_flag_builder(curr->kind), 0644);
		if (fd == -1)
		{
			ft_puterr_general(curr->filename, strerror(errno));
			return (EXIT_FAILURE);
		}
		if (dup2(fd, curr->io_number) == -1)
		{
			ft_puterr_nbr(curr->io_number, strerror(errno));
			close(fd);
			return (EXIT_FAILURE);
		}
		if (fd != curr->io_number)
			close(fd);
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}
