/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:41:30 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 23:05:15 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	builtin_env(t_tree_node *node, t_env *env);

unsigned char	builtin_env(t_tree_node *node, t_env *env)
{
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
