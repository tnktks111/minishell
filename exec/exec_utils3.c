/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:59:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 10:28:00 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		*save_std_fds(void);
void	restore_std_fds(int *saved_std_fds);
void	find_path_failure_handler(char *cmd_name, int errnum, t_env *env);
void	execve_failure_handler(char *cmd_name, int errnum, t_env *env);
void	n_wait(size_t n);

int	*save_std_fds(void)
{
	int	*saved_std_fds;

	saved_std_fds = (int *)malloc(sizeof(int) * 3);
	if (!saved_std_fds)
		return (ft_puterr_malloc());
	saved_std_fds[0] = dup(STDIN_FILENO);
	if (saved_std_fds[0] == -1)
		return (perror_string("dup()"), NULL);
	saved_std_fds[1] = dup(STDOUT_FILENO);
	if (saved_std_fds[1] == -1)
	{
		perror("dup()");
		close(saved_std_fds[0]);
		return (perror_string("dup()"), NULL);
	}
	saved_std_fds[2] = dup(STDERR_FILENO);
	if (saved_std_fds[2] == -1)
	{
		close(saved_std_fds[0]);
		close(saved_std_fds[1]);
		return (perror_string("dup()"), NULL);
	}
	return (saved_std_fds);
}

void	restore_std_fds(int *saved_std_fds)
{
	if (!saved_std_fds)
		return ;
	dup2(saved_std_fds[0], STDIN_FILENO);
	dup2(saved_std_fds[1], STDOUT_FILENO);
	dup2(saved_std_fds[2], STDERR_FILENO);
	close(saved_std_fds[0]);
	close(saved_std_fds[1]);
	close(saved_std_fds[2]);
	free(saved_std_fds);
	saved_std_fds = NULL;
}

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

void	n_wait(size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}
