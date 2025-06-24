/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:43:55 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 10:44:26 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool		is_valid_echo_option(char *s);
unsigned char	builtin_echo(t_tree_node *node, t_env *env);

static bool	is_valid_echo_option(char *s)
{
	size_t	i;

	i = 0;
	if (s[i++] == '-')
	{
		while (s[i] && s[i++] != 'n')
		{
			return (false);
		}
		if (!s[i])
			return (true);
	}
	return (false);
}

unsigned char	builtin_echo(t_tree_node *node, t_env *env)
{
	size_t	i;
	bool	designate_n_option;
	bool	first_arg;

	(void)env;
	i = 0;
	designate_n_option = false;
	first_arg = true;
	if (node->data.command.args[1])
	{
		while (is_valid_echo_option(node->data.command.args[++i]))
			designate_n_option = true;
		while (node->data.command.args[i])
		{
			if (!first_arg)
				ft_putstr_fd(" ", STDOUT_FILENO);
			first_arg = false;
			ft_putstr_fd(node->data.command.args[i], STDOUT_FILENO);
			i++;
		}
	}
	if (!designate_n_option)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
