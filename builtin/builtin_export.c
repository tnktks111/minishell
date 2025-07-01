/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:42:50 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 19:40:07 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int				_is_valid_export_key(char *s);
static unsigned char	_builtin_export_helper(char *arg, t_env *env);
unsigned char			builtin_export(t_tree_node *node, t_env *env);

static int	_is_valid_export_key(char *s)
{
	if (ft_isdigit(*s) || *s == '=' || *s == '+')
		return (false);
	while (*s)
	{
		if (*s == '+')
		{
			if (*(s + 1) && *(s + 1) == '=')
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

static unsigned char	_builtin_export_helper(char *arg, t_env *env)
{
	char	*key;
	char	*val;
	int		code;

	code = _is_valid_export_key(arg);
	if (!code)
	{
		error_not_a_valid_identifier("export", arg);
		return (EXIT_FAILURE);
	}
	else if (code == 1)
	{
		key = extract_key(arg);
		val = extract_val(arg);
		return (ft_add_key_val_pair(key, val, env));
	}
	else
	{
		key = extract_key_when_additional(arg);
		val = extract_val_when_additional(arg, key, env);
		return (ft_add_key_val_pair(key, val, env));
	}
}

unsigned char	builtin_export(t_tree_node *node, t_env *env)
{
	unsigned char	res;
	size_t			i;

	res = EXIT_SUCCESS;
	i = 0;
	if (!(node->data.command.args[1]))
		display_env(env, true);
	while (node->data.command.args[++i])
		res |= _builtin_export_helper(node->data.command.args[i], env);
	return (res);
}
