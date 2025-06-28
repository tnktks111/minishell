/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:28:23 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 16:15:25 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_search(char *key, t_env *env);
int		encode_envp(t_env *env, char *envp[]);
char	**decode_table(t_env *env, bool include_quote);

char	*ft_search(char *key, t_env *env)
{
	unsigned int	hash;
	t_env_node		*curr;

	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(env->prev_exit_status));
	hash = _hash_fnv1a(key) % HASH_SIZE;
	curr = &env->table[hash];
	if (curr->is_empty == true)
		return (ft_strdup(""));
	if (ft_strcmp(curr->key, key) == 0)
		return (ft_strdup(curr->val));
	curr = curr->next;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (ft_strdup(curr->val));
		curr = curr->next;
	}
	return (ft_strdup(""));
}

char	*shlvl_handler(char *val)
{
	int	shlvl;

	shlvl = ft_atoi_for_shlvl(val);
	if (shlvl == INT_MAX || shlvl < 0)
		shlvl = -1;
	shlvl++;
	if (shlvl > 999)
	{
		warning_shlvl_too_high(shlvl);
		shlvl = 1;
	}
	free(val);
	val = ft_itoa(shlvl);
	return (val);
}

int	register_entry_to_table(char *entry, bool *have_shlvl,
		bool *have_underscore, t_env *env)
{
	char	*key;
	char	*val;

	key = extract_key(entry);
	val = extract_val(entry);
	if (!key || !val)
		return (free(key), free(val), free_table(env), EXIT_FAILURE);
	if (ft_strcmp(key, "_") == 0)
		*have_underscore = true;
	if (ft_strcmp(key, "SHLVL") == 0)
	{
		*have_shlvl = true;
		val = shlvl_handler(val);
		if (!val)
			return (free(key), free_table(env), EXIT_FAILURE);
	}
	if (ft_add_key_val_pair(key, val, env) == EXIT_FAILURE)
		return (free_table(env), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	encode_envp(t_env *env, char *envp[])
{
	size_t	i;
	bool	have_shlvl;
	bool	have_underscore;

	_init_table(env);
	i = 0;
	env->prev_exit_status = 0;
	have_underscore = false;
	have_shlvl = false;
	while (envp[i] != NULL)
	{
		if (register_entry_to_table(envp[i++], &have_shlvl, &have_shlvl,
				env) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (have_underscore == false && ft_add_key_val_pair(ft_strdup("_"),
			ft_strdup(""), env) == EXIT_FAILURE)
		return (free_table(env), EXIT_FAILURE);
	if (have_shlvl == false && ft_add_key_val_pair(ft_strdup("SHLVL"),
			ft_strdup("1"), env) == EXIT_FAILURE)
		return (free_table(env), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	**decode_table(t_env *env, bool include_quote)
{
	char		**res;
	size_t		i;
	size_t		j;
	t_env_node	*curr;

	res = (char **)calloc(env->entry_cnt + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (i < HASH_SIZE)
	{
		curr = &env->table[i++];
		if (curr->is_empty)
			continue ;
		while (curr)
		{
			res[j] = _concatnate_key_val(curr->key, curr->val,
					include_quote);
			if (!res[j++])
				return (free_allocated_data(res, j - 1));
			curr = curr->next;
		}
	}
	return (res);
}
