/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:41:59 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 14:29:12 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_unset_key(char *s);
unsigned char	builtin_unset(t_tree_node *node, t_env *env);

static int	is_valid_unset_key(char *s)
{
	if (ft_strcmp(s, "?") == 0)
		return (1);
	while (*s)
	{
		if (!(ft_isalnum(*s) || *s == '_'))
			return (0);
		s++;
	}
	return (1);
}

unsigned char	builtin_unset(t_tree_node *node, t_env *env)
{
	size_t			i;
	unsigned char	exit_status;

	i = 0;
	exit_status = EXIT_SUCCESS;
	while (node->data.command.args[++i])
	{
		if (is_valid_unset_key(node->data.command.args[i]))
		{
			ft_remove_key(node->data.command.args[i], env);
		}
		else
		{
			error_not_a_valid_identifier("unset", node->data.command.args[i]);
			exit_status = EXIT_FAILURE;
		}
	}
	return (exit_status);
}