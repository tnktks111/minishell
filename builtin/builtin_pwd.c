/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:43:19 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/26 18:20:56 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	builtin_pwd(t_tree_node *node, t_env *env);

unsigned char	builtin_pwd(t_tree_node *node, t_env *env)
{
	char	pwd[PATH_MAX];
	char 	*envpwd;

	(void)env;
	if (node->data.command.args[1])
	{
		if (node->data.command.args[1][0] == '-')
		{
			error_invalid_option(node->data.command.args[0],
				node->data.command.args[1]);
			return (2);
		}
	}
	if (!getcwd(pwd, PATH_MAX))
	{
		envpwd = ft_search("PWD", env);
		if (!envpwd || !envpwd[0])
		{
			perror("getcwd():");
			return (EXIT_FAILURE);
		}
		ft_putendl_fd(envpwd, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}