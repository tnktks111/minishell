/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-29 05:08:51 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-29 05:08:51 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_string(char *str);
void	ft_puterr_general(char *cmd_name, char *errmsg);
void	ft_puterr_nbr(int nbr, char *errmsg);

void	perror_string(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
}

void	ft_puterr_general(char *cmd_name, char *errmsg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsg, STDERR_FILENO);
}

void	ft_puterr_nbr(int nbr, char *errmsg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putnbr_fd(nbr, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(errmsg, STDERR_FILENO);
}
