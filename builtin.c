/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 21:06:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/17 16:10:35 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char builtin_echo(t_tree_node *node, t_env *env);
unsigned char builtin_pwd(t_tree_node *node, t_env *env);
unsigned char builtin_export(t_tree_node *node, t_env *env);
unsigned char builtin_unset(t_tree_node *node, t_env *env);
unsigned char builtin_env(t_tree_node *node, t_env *env);
unsigned char builtin_exit(t_tree_node *node, t_env *env);

bool is_valid_echo_option(char *s)
{
    size_t i;

    i = 0;
    if (s[i++] == '-')
    {
        while (s[i] && s[i++] != 'n')
        {
            return false;
        }
        if (!s[i])
            return true;
    }
    return false;
}

unsigned char builtin_echo(t_tree_node *node, t_env *env)
{
    size_t i;
    bool designate_n_option;
    bool first_arg;
    
    (void)env;
    i = 0;
    designate_n_option = false;
    first_arg = true;
    while (is_valid_echo_option(node->data.command.args[++i]))
        designate_n_option = true;
    while (node->data.command.args[i])
    {
        if (!first_arg)
            ft_putstr_fd(" ", STDOUT_FILENO);
        ft_putstr_fd(node->data.command.args[i], STDOUT_FILENO);
        i++;
    }
    if (!designate_n_option)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return (EXIT_SUCCESS);
}

// unsigned char builtin_cd(t_tree_node *node, t_env *env)
// {
    
// }

unsigned char builtin_pwd(t_tree_node *node, t_env *env)
{
    char *hyphen;
    char *pwd;
    if (node->data.command.args[1])
    {
        if (node->data.command.args[1][0] == '-')
            error_invalid_option(node->data.command.args[0], node->data.command.args[1]);
            return (EXIT_FAILURE);
    }
    pwd = ft_search("PWD", env);
    ft_putendl_fd(pwd, STDOUT_FILENO);
    return (EXIT_FAILURE);
}

int is_valid_export_key(char *s)
{
    if (ft_isdigit(*s))
        return (false);
    while (*s && *s)
    {
        if (*s == '+')
        {
            if (*(s + 1) == '=')
                return (2);
            else
                return (0);
        }
        if (*s == '=')
            return (1);
        if (!(ft_isalnum(*s) || *s == '_'))
            return (0);
        s++;
    }
    return (1);
}

unsigned char builtin_export(t_tree_node *node, t_env *env)
{
    int code;
    unsigned char res;
    char *key;
    char *val;
    size_t i;

    res = EXIT_SUCCESS;
    i = 0;
    if (!(node->data.command.args[0]))
        display_env(env, true);
    while (node->data.command.args[++i])
    {
        code = is_valid_export_key(node->data.command.args[i]);
        if (!code)
        {
            error_not_a_valid_identifier("export", node->data.command.args[i]);
            res = EXIT_FAILURE;
        }
        else if (code == 1)
        {
            key = extract_key(node->data.command.args[i]);
            val = extract_val(node->data.command.args[i]);
            ft_add_key_val_pair(key, val, env);
        }
        else
        {
            key = extract_key_when_additional(node->data.command.args[i]);
            val = extract_val_when_additional(node->data.command.args[i], key, env);
            ft_add_key_val_pair(key, val, env);
        }
    }
    return res;
}

int is_valid_unset_key(char *s)
{
    while (*s)
    {
        if (!(ft_isalnum(*s) || *s == '_'))
            return 0;
        s++;
    }
    return 1;
}

unsigned char builtin_unset(t_tree_node *node, t_env *env)
{
    size_t i;
    unsigned char exit_status;

    i = 0;
    exit_status = EXIT_SUCCESS;
    while (node->data.command.args[++i])
    {
        if (is_valid_unset_key(node->data.command.args[++i]))
            ft_remove_key(node->data.command.args[i], env);
        else
        {
            error_not_a_valid_identifier("unset", node->data.command.args[i]);
            exit_status = EXIT_FAILURE;
        }
    }
    return (exit_status);
}

unsigned char builtin_env(t_tree_node *node, t_env *env)
{
    if (!node->data.command.args[0])
    {   
        display_env(env, false);
        return (EXIT_SUCCESS);
    }
    else
    {
        error_illegal_arguments(node->data.command.args[0], node->data.command.args[1]);
        return (EXIT_FAILURE);
    }
}

int ft_isnumeric(char *s)
{
    while(*s)
    {
        if (!ft_isdigit(*s))
            return (0);
        s++;
    }
    return (1);
}
long long ft_strtol(char *s, bool *error)
{
    long long sign;
    long long num;

    sign = 1;
    num = 0;
	if (ft_strcmp(s, "-9223372036854775808") == 0)
		return (LLONG_MIN);
    if (*s == '+' || *s == '-')
    {
        if (*s == '-')
            sign *= -1;
        s++;
    }
    if (!(*s) || !ft_isdigit(*s))
        *error = true;
    while (*s && ft_isdigit(*s))
    {
        if (num >= num * 10 + (*s - '0'))
            *error = true;
        num = num * 10 + (*s++ - '0');
    }
    if (*s)
        *error = true;
    return (sign * num);
}

unsigned char builtin_exit(t_tree_node *node, t_env *env)
{
    bool error;
    long long exit_status;

    error = false;
    /*numericでないときは、exitする*/
    if (!node->data.command.args[1])
        exit(0);
    exit_status = ft_strtol(node->data.command.args[1], &error);
    if (error == true)
    {
        error_numeric_arg_required("exit", node->data.command.args[1]);
        exit(255);
    }
    if (node->data.command.args[2])
    {
        error_too_many_args("exit");
        return (EXIT_FAILURE);
    }
	printf("%lld", exit_status);
    exit((unsigned char)exit_status);
}
