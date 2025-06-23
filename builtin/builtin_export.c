/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:42:50 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 10:43:11 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_export_key(char *s);
unsigned char	builtin_export(t_tree_node *node, t_env *env);

static int	is_valid_export_key(char *s)
{
	if (ft_isdigit(*s))
		return (false);
	while (*s && *s)
	{
		if (*s == '+')
		{
			if (*(s + 1) == '=')
				return (2);
			else
				return (0);
		}
		if (*s == '=')
			return (1);
		if (!(ft_isalnum(*s) || *s == '_'))
			return (0);
		s++;
	}
	return (1);
}

unsigned char	builtin_export(t_tree_node *node, t_env *env)
{
	int				code;
	unsigned char	res;
	char			*key;
	char			*val;
	size_t			i;

	res = EXIT_SUCCESS;
	i = 0;
	if (!(node->data.command.args[0]))
		display_env(env, true);
	while (node->data.command.args[++i])
	{
		code = is_valid_export_key(node->data.command.args[i]);
		if (!code)
		{
			error_not_a_valid_identifier("export", node->data.command.args[i]);
			res = EXIT_FAILURE;
		}
		else if (code == 1)
		{
			key = extract_key(node->data.command.args[i]);
			val = extract_val(node->data.command.args[i]);
			ft_add_key_val_pair(key, val, env);
		}
		else
		{
			key = extract_key_when_additional(node->data.command.args[i]);
			val = extract_val_when_additional(node->data.command.args[i], key,
					env);
			ft_add_key_val_pair(key, val, env);
		}
	}
	return (res);
}