/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:34:11 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 18:33:16 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		_modify_val_of_entry(char *val, t_env_node *node);
int		_set_key_val(char *key, char *val, t_env_node *node, t_env *env);
int		ft_add_key_val_pair(char *key, char *val, t_env *env);
int		ft_set_underscore(t_tree_node *cmd_node, t_env *env);
void	ft_remove_key(char *key, t_env *env);

int	_modify_val_of_entry(char *val, t_env_node *node)
{
	free(node->val);
	node->val = val;
	return (EXIT_SUCCESS);
}

int	_set_key_val(char *key, char *val, t_env_node *node, t_env *env)
{
	node->key = key;
	node->val = val;
	node->is_empty = false;
	env->entry_cnt++;
	return (EXIT_SUCCESS);
}

int	ft_add_key_val_pair(char *key, char *val, t_env *env)
{
	t_env_node	*curr;

	if (!key || !val)
	{
		ft_puterr_malloc();
		return (free(key), free(val), EXIT_FAILURE);
	}
	curr = &(env->table[_hash_fnv1a(key) % HASH_SIZE]);
	if (curr->is_empty == true)
		return (_set_key_val(key, val, curr, env));
	else if (ft_strcmp(curr->key, key) == 0)
		return (free(key), _modify_val_of_entry(val, curr));
	while (curr->next)
	{
		curr = curr->next;
		if (ft_strcmp(curr->key, key) == 0)
			return (free(key), _modify_val_of_entry(val, curr));
	}
	curr->next = _create_env_node(key, val, env);
	if (!curr->next)
	{
		ft_puterr_malloc();
		return (free(key), free(val), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_set_underscore(t_tree_node *cmd_node, t_env *env)
{
	char	**args;
	size_t	i;

	args = cmd_node->data.command.args;
	i = 0;
	while (args[i + 1])
		i++;
	if (ft_add_key_val_pair(ft_strdup("_"), ft_strdup(args[i]),
			env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_remove_key(char *key, t_env *env)
{
	t_env_node	*curr;
	t_env_node	*tmp;

	curr = &env->table[_hash_fnv1a(key) % HASH_SIZE];
	if (curr->is_empty)
		return ;
	else if (ft_strcmp(curr->key, key) == 0)
		return (_destroy_env_node(curr, env, false));
	while (curr->next)
	{
		if (ft_strcmp(curr->next->key, key) == 0)
		{
			tmp = curr->next;
			curr->next = tmp->next;
			_destroy_env_node(tmp, env, true);
			return ;
		}
		curr = curr->next;
	}
}
