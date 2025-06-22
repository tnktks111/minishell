/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:37:47 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 19:38:05 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

unsigned char	exec_builtin(t_tree_node *node, t_env *env)
{
	if (ft_strcmp(node->data.command.args[0], "echo") == 0)
		return (builtin_echo(node, env));
	if (ft_strcmp(node->data.command.args[0], "cd") == 0)
		return (builtin_cd(node, env));
	if (ft_strcmp(node->data.command.args[0], "pwd") == 0)
		return (builtin_pwd(node, env));
	if (ft_strcmp(node->data.command.args[0], "export") == 0)
		return (builtin_export(node, env));
	if (ft_strcmp(node->data.command.args[0], "unset") == 0)
		return (builtin_unset(node, env));
	if (ft_strcmp(node->data.command.args[0], "env") == 0)
		return (builtin_env(node, env));
	if (ft_strcmp(node->data.command.args[0], "exit") == 0)
		return (builtin_exit(node, env));
	else
		return (EXIT_FAILURE);
}