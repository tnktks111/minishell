/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:39:36 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 19:39:45 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*search "PATH" in env and return prefix table*/
char	**get_path_prefix(t_env *env);
/*check whether the cmd is builtin and exec*/
void	find_builtin(t_tree_node *cmd_node, t_env *env);
/*loop prefix table and try execve*/
void	find_path(t_tree_node *cmd_node, t_env *env);
/*check whether the cmd is builtin or not*/
bool	is_builtin(char *s);

char	**get_path_prefix(t_env *env)
{
	char	*val;
	char	**res;

	val = ft_search("PATH", env);
	if (!val)
		return (NULL);
	res = ft_split(val, ':');
	return (res);
}

void	find_builtin(t_tree_node *cmd_node, t_env *env)
{
	unsigned char	exit_status;

	if (!is_builtin(cmd_node->data.command.args[0]))
		return ;
	exit_status = exec_builtin(cmd_node, env);
	exit(exit_status);
}

void	find_path(t_tree_node *cmd_node, t_env *env)
{
	char	**prefix_table;
	char	*tmp;
	char	*tmp_path;
	int		i;

	prefix_table = get_path_prefix(env);
	i = -1;
	// printf("%s_\n", env->envp[0]);
	while (prefix_table[++i])
	{
		tmp = ft_strjoin(prefix_table[i], "/");
		tmp_path = ft_strjoin(tmp, cmd_node->data.command.args[0]);
		free(tmp);
		if (!tmp_path)
		{
			perror("malloc :");
			return ;
		}
		execve(tmp_path, cmd_node->data.command.args, env->envp);
		free(tmp_path);
	}
	free_splited_data(prefix_table);
    error_cmd_not_found(cmd_node->data.command.args[0]);
	exit(127);
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