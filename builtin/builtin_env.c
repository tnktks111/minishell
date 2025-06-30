/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:41:30 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 01:40:30 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

unsigned char	builtin_env(t_tree_node *node, t_env *env);

static int _shlvl_decrementer(t_env *env)
{
	int shlvl;
	char *val;

	val = ft_search("SHLVL", env);
	if (!val)
		return (ft_puterr_malloc(), EXIT_FAILURE);
	shlvl = ft_atoi_for_shlvl(val);
	free(val);
	if (shlvl == INT_MAX || shlvl <= 0)
		shlvl = 1;
	shlvl--;
	if (shlvl > 999)
	{
		warning_shlvl_too_high(shlvl);
		shlvl = 0;
	}
	val = ft_itoa(shlvl);
	return (ft_add_key_val_pair(ft_strdup("SHLVL"), val, env));
}

unsigned char	builtin_env(t_tree_node *node, t_env *env)
{
	if (env->in_subshell || env->have_pipe)
	{
		if (_shlvl_decrementer(env) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (!node->data.command.args[1])
	{
		display_env(env, false);
		return (EXIT_SUCCESS);
	}
	else
	{
		error_illegal_arguments(node->data.command.args[0],
			node->data.command.args[1]);
		return (EXIT_FAILURE);
	}
}
