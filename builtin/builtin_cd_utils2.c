/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:43:37 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/29 15:51:40 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bindpwd(t_env *env);
int	absolute_pathname(const char *string);
int	change_to_directory(char *path);

int	bindpwd(t_env *env)
{
	char	*oldpwd;
	char	*pwd;
	char	tmppwd[PATH_MAX];

	if (!getcwd(tmppwd, PATH_MAX))
	{
		perror("getcwd(): ");
		return (2);
	}
	pwd = ft_strdup(tmppwd);
	if (!pwd)
	{
		perror("malloc(): ");
		return (2);
	}
	oldpwd = ft_search("PWD", env);
	ft_add_key_val_pair("OLDPWD", oldpwd, env);
	ft_add_key_val_pair("PWD", pwd, env);
	return (0);
}

int	absolute_pathname(const char *string)
{
	if (string == 0 || *string == '\0')
		return (0);
	if (string[0] == '/')
		return (1);
	if (string[0] == '.' && string[1] == '/')
		return (1);
	if (string[0] == '.' && string[1] == '.' && string[2] == '/')
		return (1);
	return (0);
}

int	change_to_directory(char *path)
{
	char	*abs_path;

	abs_path = make_absolute(path);
	if (chdir(abs_path) < 0)
	{
		return (0);
	}
	return (1);
}
