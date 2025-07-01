/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:40:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 15:47:38 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static long long	ft_strtol_for_exit(char *s, bool *error);
unsigned char		builtin_exit(t_tree_node *node, t_env *env, int *saved_std_fds);

long long	ft_strtol_for_exit(char *s, bool *error)
{
	long long	sign;
	long long	num;

	sign = 1;
	num = 0;
	if (ft_strcmp(s, "-9223372036854775808") == 0)
		return (LLONG_MIN);
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign *= -1;
		s++;
	}
	if (!(*s) || !ft_isdigit(*s))
		*error = true;
	while (*s && ft_isdigit(*s))
	{
		if (num > num * 10 + (*s - '0'))
			*error = true;
		num = num * 10 + (*s++ - '0');
	}
	if (*s)
		*error = true;
	return (sign * num);
}

void	free_for_builtin_exit(t_env *env, unsigned char exit_status, int *saved_std_fds)
{
	restore_std_fds(saved_std_fds);
	free_all(env);
	exit(exit_status);
}

unsigned char	builtin_exit(t_tree_node *node, t_env *env, int *saved_std_fds)
{
	bool		error;
	long long	exit_status;

	error = false;
	if (env->is_child == false)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (!node->data.command.args[1])
		free_for_builtin_exit(env, env->prev_exit_status, saved_std_fds);
	exit_status = ft_strtol_for_exit(node->data.command.args[1], &error);
	if (error == true)
	{
		error_numeric_arg_required("exit", node->data.command.args[1]);
		free_for_builtin_exit(env, 2, saved_std_fds);
	}
	if (node->data.command.args[2])
	{
		error_too_many_args("exit");
		return (EXIT_FAILURE);
	}
	free_for_builtin_exit(env, (unsigned char)exit_status, saved_std_fds);
	return (EXIT_SUCCESS);
}
