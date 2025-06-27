/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:34:11 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/24 16:24:53 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int _modify_val_of_entry(char *val, t_env_node *node);
int _set_key_val(char *key, char *val, t_env_node *node, t_env *env);
int ft_add_key_val_pair(char *key, char *val, t_env *env);
int ft_set_underscore(t_tree_node *cmd_node, t_env *env);
void ft_remove_key(char *key, t_env *env);

int _modify_val_of_entry(char *val, t_env_node *node)
{
    node->val = val;
    return (EXIT_SUCCESS);
}

int _set_key_val(char *key, char *val, t_env_node *node, t_env *env)
{
    node->key = key;
    node->val = val;
    node->is_empty = false;
    env->entry_cnt++;
    return (EXIT_SUCCESS);
}

int ft_add_key_val_pair(char *key, char *val, t_env *env)
{
    t_env_node *curr;

    curr = &(env->table[_hash_fnv1a(key) % HASH_SIZE]);
    if (curr->is_empty == true)
        return(_set_key_val(key, val, curr, env));
    else if (ft_strcmp(curr->key, key) == 0)
        return(_modify_val_of_entry(val, curr));    
    while (curr->next)
    {
        curr = curr->next;
        if (ft_strcmp(curr->key, key) == 0)
            return (_modify_val_of_entry(val, curr));
    }
    curr->next = _create_env_node(key, val, env);
    if (!curr->next)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

int ft_set_underscore(t_tree_node *cmd_node, t_env *env)
{
    char **args;
    char *last_arg;
    size_t i;

    args = cmd_node->data.command.args;
    i = 0;
    while (args[i + 1])
        i++;
    last_arg = ft_strdup(args[i]);
    if (!last_arg)
        return (EXIT_FAILURE);
    ft_add_key_val_pair("_", last_arg, env);
    return (EXIT_SUCCESS);
}

void ft_remove_key(char *key, t_env *env)
{
    t_env_node *curr;
    t_env_node *tmp;

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


// int main(int ac, char *av[], char *envp[])
// {
//     t_env env;
//     char **res;
//     (void)ac;
//     (void)av;
//     int i = 0;
//     printf("original envp are as follows\n");
//     while(envp[i])
//     {
//         printf("%s\n", envp[i]);
//         i++;
//     }
//     printf("-----------------------------------------------------------------\n");
//     printf("entry cnt: %d\n", i);
//     encode_envp(&env, envp);
//     printf("encode success\n");
//     printf("ft_search:\n");
//     printf("%s\n", ft_search("LOGNAME", &env));
//     printf("ft_remove_key:\n");
//     ft_remove_key("LOGNAME", &env);
//     printf("%s\n", ft_search("LOGNAME", &env));
//     printf("ft_add_key_val_pair:\n");
//     ft_add_key_val_pair(ft_strdup("LOGNAME"), ft_strdup("4242422"), &env);
//     printf("%s\n", ft_search("LOGNAME", &env));
//     printf("-----------------------------------------------------------------\n");
//     printf("----------------------decode_test----------------------\n");
//     res = decode_table(&env);
//     i = 0;
//     while(res[i])
//     {
//         printf("%s\n", res[i]);
//         i++;
//     }
// }