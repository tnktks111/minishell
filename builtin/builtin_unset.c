/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:41:59 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 14:49:33 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static bool		is_valid_option(char *s);
unsigned char	builtin_unset(t_tree_node *node, t_env *env);

static bool	is_valid_option(char *s)
{
	if (s[0] == '-' && s[1] && s[1] != '-')
		return (false);
	if (s[0] == '-' && s[1] == '-' && s[2])
		return (false);
	return (true);
}

unsigned char	builtin_unset(t_tree_node *node, t_env *env)
{
	size_t	i;

	if (node->data.command.args[1]
		&& !is_valid_option(node->data.command.args[1]))
	{
		error_invalid_option(node->data.command.args[0],
			node->data.command.args[1]);
		return (2);
	}
	i = 0;
	while (node->data.command.args[++i])
		ft_remove_key(node->data.command.args[i], env);
	return (0);
}
