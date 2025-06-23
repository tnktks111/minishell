/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:43 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 13:49:18 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool is_same_in_modern_locale(char c1, char c2);
static int ft_strcmp_for_export(char *s1, char *s2);
void display_env(t_env *env, bool print_declare);

static bool is_same_in_modern_locale(char c1, char c2)
{
    return (ft_tolower(c1) == ft_tolower(c2));
}

static int ft_strcmp_for_export(char *s1, char *s2)
{
    while (*s1 && *s2 && is_same_in_modern_locale(*s1, *s2) && *s1 != '=' && *s2 != '=')
    {
        s1++;
        s2++;
    }
    if (*s1 == '=') 
        return (-(*s2));
    if (*s2 == '=')
        return (*s1);
    if (!(*s1) || !(*s2))
        return (*s1 - *s2);
    if (*s1 == '_')
        return (-(*s2));
    if (*s2 == '_')
        return (*s1);
    return (ft_tolower(*s1) - ft_tolower(*s2));
}

void display_env(t_env *env, bool print_declare)
{
    size_t i;
    char **env_tab;

    env_tab = decode_table(env, print_declare);
    if (print_declare == true)
        ft_sort_string_tab(env_tab, ft_strcmp_for_export);
    i = 0;
    while(env_tab[i])
    {
        if (print_declare == true && ft_strncmp(env_tab[i], "_=", 2) == 0)
        {
            i++;
            continue;
        }
        if (print_declare == true)
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putendl_fd(env_tab[i++], STDOUT_FILENO);
    }
    free_splited_data(env_tab);
    return ;
}