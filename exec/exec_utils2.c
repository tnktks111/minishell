/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:39:36 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 15:21:52 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*search "PATH" in env and return prefix table*/
char		**get_path_prefix(t_env *env);
/*check whether the cmd is builtin and exec*/
void		find_builtin(t_tree_node *cmd_node, t_env *env);
/*loop prefix table and try execve*/
void		find_path(char **args, t_env *env);
/*check whether the cmd is builtin or not*/
bool		is_builtin(char *s);

char	**get_path_prefix(t_env *env)
{
	char	*val;
	char	**res;

	val = ft_search("PATH", env);
	if (!val)
	{
		ft_puterr_malloc();
		free_for_exit(env, EXIT_FAILURE);
	}
	res = ft_split(val, ':');
	free(val);
	if (!res)
	{
		ft_puterr_malloc();
		free_for_exit(env, EXIT_FAILURE);
	}
	return (res);
}

void	find_builtin(t_tree_node *cmd_node, t_env *env)
{
	unsigned char	exit_status;

	if (ft_strchr(cmd_node->data.command.args[0], '/'))
		return ;
	if (!is_builtin(cmd_node->data.command.args[0]))
		return ;
	exit_status = exec_builtin(cmd_node, env, NULL);
	free_for_exit(env, exit_status);
}

static int	try_one_prefix(char *prefix, char **args, t_env *env,
		int *last_errno)
{
	char	*tmp_path;

	tmp_path = join_path(prefix, args[0]);
	if (!tmp_path)
	{
		perror_string("malloc");
		return (EXIT_FAILURE);
	}
	execve(tmp_path, args, env->envp);
	if (errno != ENOENT)
		*last_errno = errno;
	free(tmp_path);
	return (EXIT_SUCCESS);
}

void	find_path(char **args, t_env *env)
{
	char	**prefix_table;
	int		i;
	int		l_errno;

	if (ft_strchr(args[0], '/'))
		return ;
	prefix_table = get_path_prefix(env);
	l_errno = ENOENT;
	if (!prefix_table[0])
	{
		free_splited_data(prefix_table);
		ft_puterr_general(args[0], strerror(ENOENT));
		free_for_exit(env, 126);
	}
	i = -1;
	while (prefix_table[++i])
	{
		if (try_one_prefix(prefix_table[i], args, env,
				&l_errno) == EXIT_FAILURE)
		{
			free_splited_data(prefix_table);
			free_for_exit(env, EXIT_FAILURE);
		}
	}
	free_splited_data(prefix_table);
	find_path_failure_handler(args[0], l_errno, env);
}

bool	is_builtin(char *s)
{
	if (!s)
		return (false);
	if (ft_strcmp(s, "echo") == 0)
		return (true);
	if (ft_strcmp(s, "cd") == 0)
		return (true);
	if (ft_strcmp(s, "pwd") == 0)
		return (true);
	if (ft_strcmp(s, "export") == 0)
		return (true);
	if (ft_strcmp(s, "unset") == 0)
		return (true);
	if (ft_strcmp(s, "env") == 0)
		return (true);
	if (ft_strcmp(s, "exit") == 0)
		return (true);
	return (false);
}
