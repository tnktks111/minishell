/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:14:27 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/24 16:25:50 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_redirection(t_redirect *redirect)
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
		{
			printf("minishell: %s: %s\n", curr->filename, strerror(errno));
			return(EXIT_FAILURE);
		}
		if (dup2(fd, curr->io_number) == -1)
		{
			printf("minishell: %d: %s\n", curr->io_number, strerror(errno));
			close(fd);
			return(EXIT_FAILURE);
		}
		if (fd != curr->io_number)
			close(fd);
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}
