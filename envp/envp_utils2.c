/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:32:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 14:55:10 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void			_init_table(t_env *env);
unsigned int	_hash_fnv1a(char *str);
t_env_node		*_create_env_node(char *key, char *val, t_env *env);
void			_destroy_env_node(t_env_node *node, t_env *env,
					bool envp_is_malloced);
void			free_table(t_env *env);

unsigned int	_hash_fnv1a(char *str)
{
	unsigned int	hash;

	hash = 0x811c9dc5;
	while (*str)
	{
		hash ^= (uint8_t)*str++;
		hash *= 0x01000193;
	}
	return (hash);
}

void	_init_table(t_env *env)
{
	size_t	i;

	i = 0;
	env->entry_cnt = 0;
	while (i < HASH_SIZE)
	{
		env->table[i].is_empty = true;
		env->table[i].key = NULL;
		env->table[i].val = NULL;
		env->table[i].next = NULL;
		i++;
	}
}

void	free_table(t_env *env)
{
	size_t		table_idx;
	t_env_node	*curr;
	t_env_node	*temp;
	bool		envp_is_malloced;

	table_idx = 0;
	while (table_idx < HASH_SIZE)
	{
		envp_is_malloced = false;
		curr = &env->table[table_idx];
		table_idx++;
		if (curr->is_empty)
			continue ;
		while (curr)
		{
			temp = curr;
			curr = curr->next;
			_destroy_env_node(temp, env, envp_is_malloced);
			envp_is_malloced = true;
		}
	}
	if (env->envp_is_malloced == true)
		free_splited_data(env->envp);
}

t_env_node	*_create_env_node(char *key, char *val, t_env *env)
{
	t_env_node	*newnode;

	newnode = (t_env_node *)malloc(sizeof(t_env_node));
	if (!newnode)
		return (NULL);
	newnode->key = key;
	newnode->val = val;
	newnode->is_empty = false;
	newnode->next = NULL;
	env->entry_cnt++;
	return (newnode);
}

void	_destroy_env_node(t_env_node *node, t_env *env, bool envp_is_malloced)
{
	free(node->key);
	free(node->val);
	if (envp_is_malloced)
		free(node);
	else
		node->is_empty = true;
	env->entry_cnt--;
}
