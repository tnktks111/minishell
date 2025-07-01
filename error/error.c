/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:22:41 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 14:24:11 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	error_invalid_option(char *cmd_name, char *arg);
void	builtin_error(char *cmd_name, char *arg, char *errmsg);
void	warning_shlvl_too_high(int shlvl);
void	error_heredoc_delimited_by_eof(char *delimiter);

void	error_invalid_option(char *cmd_name, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putchar_fd(arg[0], STDERR_FILENO);
	ft_putchar_fd(arg[1], STDERR_FILENO);
	ft_putstr_fd(": invalid option\n", STDERR_FILENO);
}

void	builtin_error(char *cmd_name, char *arg, char *errmsg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsg, STDERR_FILENO);
}

void	warning_shlvl_too_high(int shlvl)
{
	ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
	ft_putnbr_fd(shlvl, STDERR_FILENO);
	ft_putendl_fd(") too high, resetting to 1", STDERR_FILENO);
}

void	error_heredoc_delimited_by_eof(char *delimiter)
{
	ft_putstr_fd("minishell: warning:", STDERR_FILENO);
	ft_putstr_fd(" here-document delimited by end-of-file (wanted '",
		STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putendl_fd("`)", STDERR_FILENO);
}
