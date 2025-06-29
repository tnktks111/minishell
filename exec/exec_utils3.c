/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:59:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/29 18:02:00 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_path_failure_handler(char *cmd_name, int errnum, t_env *env);
void	execve_failure_handler(char *cmd_name, int errnum, t_env *env);

void	find_path_failure_handler(char *cmd_name, int errnum, t_env *env)
{
	if (errnum == ENOENT)
	{
		ft_puterr_general(cmd_name, "command not found");
		free_for_exit(env, 127);
	}
	else
	{
		ft_puterr_general(cmd_name, strerror(errnum));
		free_for_exit(env, 127);
	}
}

void	execve_failure_handler(char *cmd_name, int errnum, t_env *env)
{
	if (is_directory(cmd_name))
	{
		ft_puterr_general(cmd_name, "Is a directory");
		free_for_exit(env, 126);
	}
	else if (errnum == EACCES)
	{
		ft_puterr_general(cmd_name, "Permission denied");
		free_for_exit(env, 126);
	}
	else if (errnum == ENOENT)
	{
		ft_puterr_general(cmd_name, "No such file or directory");
		free_for_exit(env, 127);
	}
	else
	{
		ft_puterr_general(cmd_name, strerror(errnum));
		free_for_exit(env, 127);
	}
}
