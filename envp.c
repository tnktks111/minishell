/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:26:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/13 19:21:03 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// /*単テスト用*/
// #include <stdio.h>
// int ft_strcmp(char *s1, char *s2);
// char				*ft_strdup(char *s1);
// char				*ft_strjoin(char *s1, char *s2);
// char				*ft_strchr(char *s, int c);

/*便利関数*/
char *extract_key(char *s);
char *extract_val(char *s);
char *concatnate_key_val(char *key, char *val);
/*ハッシュテーブル初期化・生成・文字配列化*/
void init_table(t_env *env);
int encode_envp(t_env *env, char *envp[]);
char **decode_table(t_env *env);
/* ハッシュ値生成*/
unsigned int hash_fnv1a(char *str);
/* ハッシュテーブル検索*/
char *ft_search(char *key, t_env *env);
/* チェーンノード生成・削除*/
t_env_node *create_env_node(char *key, char *val, t_env *env);
void destroy_env_node(t_env_node *node, t_env *env, bool is_malloced);
/*チェーンノード編集*/
int modify_val_of_entry(char *val, t_env_node *node);
int set_key_val(char *key, char *val, t_env_node *node, t_env *env);
/* エントリーの追加・削除*/
int ft_add_key_val_pair(char *key, char *val, t_env *env);
void ft_remove_key(char *key, t_env *env);

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

char *extract_key(char *s)
{
    char *tmp;
    char *res;
    char *sep;

    tmp = ft_strdup(s);
    if (!tmp)
        return (NULL);
    sep = ft_strchr(tmp, '=');
    *sep = 0;
    res = ft_strdup(tmp);
    free(tmp);
    return (res);
}

char *extract_val(char *s)
{
    char *res;

    res = ft_strdup(ft_strchr(s, '=') + 1);
    return (res);
}

char *concatnate_key_val(char *key, char *val)
{
    char *tmp;
    char *res;

    tmp = ft_strjoin(key, "=");
    if(!tmp)
        return (NULL);
    res = ft_strjoin(tmp, val);
    free(tmp);
    return (res);
}

void init_table(t_env *env)
{
    size_t i;
    
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

int encode_envp(t_env *env, char *envp[])
{
    size_t i;
    char *key;
    char *val;

    init_table(env);
    i = 0;
    while (envp[i] != NULL)
    {
        key = extract_key(envp[i]);
        val = extract_val(envp[i]);
        if (!key || !val)
        {
            free(key);
            free(val);
            return (EXIT_FAILURE);
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
            res[res_idx] = concatnate_key_val(curr->key, curr->val);
            if (!res[res_idx])
                return (NULL);
            res_idx++;
            curr = curr->next;
        }
    }
    res[res_idx] = NULL;
    return (res);
}

unsigned int hash_fnv1a(char *str)
{
    unsigned int hash;
    
    hash = 0x811c9dc5;
    while (*str)
    {
        hash ^= (uint8_t)*str++;
        hash *= 0x01000193;
    }
    return hash;
}

char *ft_search(char *key, t_env *env)
{
    unsigned int hash;
    t_env_node *curr;

    hash = hash_fnv1a(key) % HASH_SIZE;
    curr = &env->table[hash];
    if (curr->is_empty == true)
        return (NULL);
    if (ft_strcmp(curr->key, key) == 0)
        return (curr->val);
    curr = curr->next;
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
            return (curr->val);
        curr = curr->next;
    }
    return (NULL);
}

t_env_node *create_env_node(char *key, char *val, t_env *env)
{
    t_env_node *newnode;

    newnode = (t_env_node *)malloc(sizeof(t_env_node));
    newnode->key = key;
    newnode->val = val;
    newnode->is_empty = false;
    env->entry_cnt++;
    return (newnode);
}

void destroy_env_node(t_env_node *node, t_env *env, bool is_malloced)
{
    free(node->key);
    free(node->val);
    if (is_malloced)
        free(node);
    else
        node->is_empty = true;
    env->entry_cnt--;
}

int modify_val_of_entry(char *val, t_env_node *node)
{
    node->val = val;
    return (EXIT_SUCCESS);
}

int set_key_val(char *key, char *val, t_env_node *node, t_env *env)
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

    curr = &(env->table[hash_fnv1a(key) % HASH_SIZE]);
    if (curr->is_empty == true)
        return(set_key_val(key, val, curr, env));
    else if (ft_strcmp(curr->key, key) == 0)
        return(modify_val_of_entry(val, curr));    
    while (curr->next)
    {
        curr = curr->next;
        if (ft_strcmp(curr->key, key) == 0)
            return (modify_val_of_entry(val, curr));
    }
    curr->next = create_env_node(key, val, env);
    if (!curr->next)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

void ft_remove_key(char *key, t_env *env)
{
    t_env_node *curr;
    t_env_node *tmp;

    curr = &env->table[hash_fnv1a(key) % HASH_SIZE];
    if (curr->is_empty)
        return ;
    else if (ft_strcmp(curr->key, key) == 0)
        return (destroy_env_node(curr, env, false));
    while (curr->next)
    {
        if (ft_strcmp(curr->next->key, key) == 0)
        {
            tmp = curr->next;
            curr->next = tmp->next;
            destroy_env_node(curr->next, env, true);
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