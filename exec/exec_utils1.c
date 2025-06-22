/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:39:49 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 19:54:52 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_pipefd(t_pipefd *fd, t_tree_node *node, bool is_start);
int status_handler(int status);

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

int status_handler(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			return (-1);
		}
		if (WTERMSIG(status) == SIGQUIT)
		{
			ft_putstr_fd("Quit", STDERR_FILENO);
			// if (WCOREDUMP(wait_status))
			// 	ft_putstr_fd(" (core dumped)", STDERR_FILENO);
		}
		return (128 + WTERMSIG(status));
	}
	return (EXIT_FAILURE);
}