/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:28:23 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/27 19:42:52 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_search(char *key, t_env *env);
int encode_envp(t_env *env, char *envp[]);
char **decode_table(t_env *env, bool include_quote);

char *ft_search(char *key, t_env *env)
{
    unsigned int hash;
    t_env_node *curr;

    if(ft_strcmp(key, "?") == 0)
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

int encode_envp(t_env *env, char *envp[])
{
    size_t i;
    char *key;
    char *val;
    int shlvl;
    bool have_underscore;

    _init_table(env);
    i = 0;
    have_underscore = false;
    while (envp[i] != NULL)
    {
        key = extract_key(envp[i]);
        val = extract_val(envp[i]);
        if (!key || !val)
        {
            free(key);
            free(val);
            free_table(env);
            return (EXIT_FAILURE);
        }
        if (ft_strcmp(key, "_") == 0)
            have_underscore = true;
        if (ft_strcmp(key, "SHLVL") == 0)
        {
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
        }
        ft_add_key_val_pair(key, val, env);
        i++;
    }
    if (have_underscore == false)
        ft_add_key_val_pair(ft_strdup("_"), ft_strdup(""), env);
    return (EXIT_SUCCESS);
}

char **decode_table(t_env *env, bool include_quote)
{
    char **res;
    size_t table_idx;
    size_t res_idx;
    t_env_node *curr;

    res = (char **)malloc(sizeof(char *) * (env->entry_cnt + 1));
    if (!res)
        return (NULL);
    table_idx = 0;
    res_idx = 0;
    while (table_idx < HASH_SIZE)
    {
        curr = &env->table[table_idx];
        table_idx++;
        if (curr->is_empty)
            continue;
        while(curr)
        {
            res[res_idx] = _concatnate_key_val(curr->key, curr->val, include_quote);
			if (!res[res_idx])
                return(free_allocated_data(res, res_idx));
            res_idx++;
            curr = curr->next;
		}
    }
    res[res_idx] = NULL;
    return (res);
}
