/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:35:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 11:07:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int		_try_one_cdpath(char *cdpath, char *dirname,
					bool *find_success);
static int		_find_cdpath(char *dirname, t_env *env, bool *find_success);
static int		_edge_case_handler(t_tree_node *cmd_node, t_env *env,
					bool *print_path, char **dirname);
static char		*_decide_dirname(t_tree_node *cmd_node, t_env *env,
					bool *print_path, bool *find_success);
unsigned char	builtin_cd(t_tree_node *cmd_node, t_env *env);

static int	_try_one_cdpath(char *cdpath, char *dirname, bool *find_success)
{
	char	*tmp;

	tmp = join_path(cdpath, dirname);
	if (!tmp)
		return (2);
	if (change_to_directory(tmp))
	{
		*find_success = true;
		if (cdpath[0])
			printf("%s\n", tmp);
		free(tmp);
		return (0);
	}
	else
	{
		free(tmp);
		return (1);
	}
}

static int	_find_cdpath(char *dirname, t_env *env, bool *find_success)
{
	char	*cdpath;
	char	**cdpath_arr;
	size_t	i;

	cdpath = ft_search("CDPATH", env);
	if (!cdpath)
		return (free(dirname), 2);
	if (!cdpath[0])
		return (free(cdpath), 1);
	cdpath_arr = ft_split(cdpath, ':');
	if (!cdpath_arr)
		return (free(dirname), free(cdpath), 2);
	i = 0;
	while (cdpath_arr[i])
	{
		if (_try_one_cdpath(cdpath_arr[i++], dirname, find_success) == 2)
			return (free_for_find_cdpath(dirname, cdpath, cdpath_arr), 2);
		if (*find_success == true)
			return (free_for_find_cdpath(dirname, cdpath, cdpath_arr), 0);
	}
	return (free_for_find_cdpath(NULL, cdpath, cdpath_arr), 1);
}

static int	_edge_case_handler(t_tree_node *cmd_node, t_env *env,
		bool *print_path, char **dirname)
{
	static const char	*target[3] = {"HOME", NULL, "OLDPWD"};
	static const char	*errmsg[3] = {"HOME not set", "too many arguments",
		"OLDPWD not set"};
	size_t				code;

	if (!cmd_node->data.command.args[1])
		code = 0;
	else if (cmd_node->data.command.args[2])
		code = 1;
	else if (ft_strcmp(cmd_node->data.command.args[1], "-") == 0)
	{
		code = 2;
		*print_path = true;
	}
	else
		return (1);
	if (target[code])
		*dirname = ft_search((char *)target[code], env);
	else
		*dirname = NULL;
	if (!(*dirname) || !(*dirname)[0])
		return (free(*dirname), *dirname = NULL, builtin_error("cd", NULL,
				(char *)errmsg[code]), 2);
	return (0);
}

static char	*_decide_dirname(t_tree_node *cmd_node, t_env *env,
		bool *print_path, bool *find_success)
{
	char	cwd[PATH_MAX];
	char	*dirname;
	int		code;
	char	*tmp;

	if (!getcwd(cwd, PATH_MAX))
		return (perror("getcwd(): "), NULL);
	code = _edge_case_handler(cmd_node, env, print_path, &dirname);
	if (code == 1)
	{
		dirname = ft_strdup(cmd_node->data.command.args[1]);
		if (!dirname)
			return (ft_puterr_malloc());
		if (!absolute_pathname(dirname) && _find_cdpath(dirname, env,
				find_success) != 1)
			return (NULL);
		if (dirname[0] != '/')
		{
			tmp = join_path(cwd, dirname);
			if (change_to_directory(tmp))
				return (free(tmp), free(dirname), *find_success = true, NULL);
			free(tmp);
		}
	}
	return (dirname);
}

unsigned char	builtin_cd(t_tree_node *cmd_node, t_env *env)
{
	char	*dirname;
	bool	printpath;
	bool	find_success;

	printpath = false;
	find_success = false;
	dirname = _decide_dirname(cmd_node, env, &printpath, &find_success);
	if (!dirname)
	{
		if (find_success)
			return (bindpwd(env));
		else
			return (1);
	}
	if (change_to_directory(dirname))
	{
		if (printpath)
			printf("%s\n", dirname);
		free(dirname);
		return (bindpwd(env));
	}
	return (builtin_error("cd", dirname, strerror(errno)), free(dirname), 1);
}
