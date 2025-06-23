/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:28:23 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 10:23:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void display_env(t_env *env, bool print_declare);
char *ft_search(char *key, t_env *env);
int encode_envp(t_env *env, char *envp[]);
char **decode_table(t_env *env);

void display_env(t_env *env, bool print_declare)
{
    size_t table_idx;
    t_env_node *curr;
    char *print_str;

    table_idx = 0;
    while (table_idx < HASH_SIZE)
    {
        curr = &env->table[table_idx];
        table_idx++;
        if (curr->is_empty)
            continue;
        while(curr)
        {
            if (print_declare)
                ft_putstr_fd("declare -x ", STDOUT_FILENO);
            print_str = _concatnate_key_val(curr->key, curr->val, print_declare);
            if (!print_str)
                return;
            ft_putendl_fd(print_str, STDOUT_FILENO);
            curr = curr->next;
        }
    }
}

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
        return (curr->val);
    curr = curr->next;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
            return (curr->val);
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

    _init_table(env);
    i = 0;
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
    return (EXIT_SUCCESS);
}

char **decode_table(t_env *env)
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
            res[res_idx] = _concatnate_key_val(curr->key, curr->val, false);
			if (!res[res_idx])
                return(free_allocated_data(res, res_idx));
            res_idx++;
            curr = curr->next;
		}
    }
    res[res_idx] = NULL;
    return (res);
}

