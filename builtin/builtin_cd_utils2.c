/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:43:37 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 17:18:27 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		bindpwd(t_env *env);
int		absolute_pathname(const char *string);
int		change_to_directory(char *path);
void	free_for_find_cdpath(char *s1, char *s2, char **splitted_data);

int	bindpwd(t_env *env)
{
	char	*oldpwd;
	char	*pwd;
	char	tmppwd[PATH_MAX];

	if (!getcwd(tmppwd, PATH_MAX))
	{
		perror_string("getcwd()");
		return (2);
	}
	pwd = ft_strdup(tmppwd);
	oldpwd = ft_search("PWD", env);
	if (ft_add_key_val_pair(ft_strdup("OLDPWD"), oldpwd, env) == EXIT_FAILURE)
		return (2);
	if (ft_add_key_val_pair(ft_strdup("PWD"), pwd, env) == EXIT_FAILURE)
		return (2);
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
	if (!abs_path)
		return (0);
	if (chdir(abs_path) < 0)
	{
		free(abs_path);
		return (0);
	}
	free(abs_path);
	return (1);
}

void	free_for_find_cdpath(char *s1, char *s2, char **splitted_data)
{
	free(s1);
	free(s2);
	free_splited_data(splitted_data);
}