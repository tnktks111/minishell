/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 23:26:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/17 16:35:05 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*便利関数*/
char *extract_key(char *s);
char *extract_val(char *s);
char *extract_key_when_additional(char *s);
char *extract_val_when_additional(char *s, char *key, t_env *env);
char *_concatnate_key_val(char *key, char *val, bool have_quote);
void display_env(t_env *env, bool print_declare);
/*ハッシュテーブル初期化・生成・文字配列化*/
void _init_table(t_env *env);
int encode_envp(t_env *env, char *envp[]);
char **decode_table(t_env *env);
/* ハッシュ値生成*/
unsigned int _hash_fnv1a(char *str);
/* ハッシュテーブル検索*/
char *ft_search(char *key, t_env *env);
/* チェーンノード生成・削除*/
t_env_node *_create_env_node(char *key, char *val, t_env *env);
void _destroy_env_node(t_env_node *node, t_env *env, bool is_malloced);
/*チェーンノード編集*/
int _modify_val_of_entry(char *val, t_env_node *node);
int _set_key_val(char *key, char *val, t_env_node *node, t_env *env);
/* エントリーの追加・削除*/
int ft_add_key_val_pair(char *key, char *val, t_env *env);
void ft_remove_key(char *key, t_env *env);
/* ハッシュテーブルのリソース解放 */
void free_table(t_env *env);

// char **free_allocated_data(char **datas, size_t allocated)
// {
//     size_t i;

//     i = 0;
//     while (i < allocated)
//         free(datas[i++]);
//     free(datas);
//     return (NULL);
// }

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

char *extract_key_when_additional(char *s)
{
    char *tmp;
    char *res;
    char *sep;

    tmp = ft_strdup(s);
    if (!tmp)
        return (NULL);
    sep = ft_strchr(tmp, '+');
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

char *extract_val_when_additional(char *s, char *key, t_env *env)
{
    char *old_val;
    char *additional_val;
    char *res;

    old_val = ft_search(key, env);
    if (!old_val)
        return (NULL);
    additional_val = ft_strdup(ft_strchr(s, '=') + 1);
    if (!additional_val)
    {
        free(old_val);
        return (NULL);
    }
    res = ft_strjoin(old_val, additional_val);
    free(old_val);
    free(additional_val);
    return (res);
}

char *_concatnate_key_val(char *key, char *val, bool have_quote)
{
    char *tmp;
    char *tmp2;
    char *res;

    if (have_quote)
    {
        tmp = ft_strjoin(key, "=\"");
        if(!tmp)
            return (NULL);
        tmp2 = ft_strjoin(tmp, val);
        free(tmp);
        if (!tmp2)
            return (NULL);
        res = ft_strjoin(tmp2, "\"");
        free(tmp2);
    }
    else
    {
        tmp = ft_strjoin(key, "=");
        if(!tmp)
            return (NULL);
        res = ft_strjoin(tmp, val);
        free(tmp);
    }
    return (res);
}

void _init_table(t_env *env)
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
    res[env->entry_cnt] = NULL;
    return (res);
}

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
            print_str = _concatnate_key_val(curr->key, curr->val, true);
            if (!print_str)
                return;
            ft_putendl_fd(print_str, STDOUT_FILENO);
            curr = curr->next;
        }
    }
}

unsigned int _hash_fnv1a(char *str)
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

t_env_node *_create_env_node(char *key, char *val, t_env *env)
{
    t_env_node *newnode;

    newnode = (t_env_node *)malloc(sizeof(t_env_node));
    newnode->key = key;
    newnode->val = val;
    newnode->is_empty = false;
    env->entry_cnt++;
    return (newnode);
}

void _destroy_env_node(t_env_node *node, t_env *env, bool is_malloced)
{
    free(node->key);
    free(node->val);
    if (is_malloced)
        free(node);
    else
        node->is_empty = true;
    env->entry_cnt--;
}

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
            _destroy_env_node(curr->next, env, true);
            return ;
        }
        curr = curr->next;
    }
}

void free_table(t_env *env)
{
    size_t table_idx;
    t_env_node *curr;
    t_env_node *temp;
    bool is_malloced;

    table_idx = 0;
    while (table_idx < HASH_SIZE)
    {
        is_malloced = false;
        curr = &env->table[table_idx];
        table_idx++;
        if (curr->is_empty)
            continue;
        while(curr)
        {
            temp = curr;
            curr = curr->next;
            _destroy_env_node(temp, env, is_malloced);
            is_malloced = true;
        }
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