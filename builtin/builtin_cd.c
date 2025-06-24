/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:35:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/24 16:24:11 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int bindpwd(t_env *env);
static int absolute_pathname (const char *string);
char *make_absolute(char *path);
int change_to_directory(char *path);
char *concatnate_paths(char *s1, char *s2);
unsigned char builtin_cd(t_tree_node *cmd_node, t_env *env);

static int bindpwd(t_env *env)
{
    char *oldpwd;
    char *pwd;
    char tmppwd[PATH_MAX];

    if (!getcwd(tmppwd, PATH_MAX))
    {
        perror("getcwd(): ");
        return (2);
    }
    pwd = ft_strdup(tmppwd);
    if (!pwd)
    {
        perror("malloc(): ");
        return (2);
    }
    oldpwd = ft_search("PWD", env);
    ft_add_key_val_pair("OLDPWD", oldpwd, env);
    ft_add_key_val_pair("PWD", pwd, env);
    return (0);
}

static int absolute_pathname (const char *string)
{
    if (string == 0 || *string == '\0')
        return (0);
    if (string[0] == '/')
        return (1);
    if (string[0] == '.' && string[1] == '/')	/* . and ./ */
        return (1);
    if (string[0] == '.' && string[1] == '.' && string[2] == '/')
        return (1);
    return (0);
}

char *make_absolute(char *path)
{
    char *abs_path;
    char **path_sections;
    int unused_section[PATH_MAX];
    int i;
    int j;
    size_t len;
    char *p_dst;

    ft_bzero((void *)unused_section, sizeof(int) * PATH_MAX);
    path_sections = ft_split(path, '/');
    if (!path_sections)
        return (NULL);
    i = 0;
    while (path_sections[i])
    {
        if (ft_strcmp(path_sections[i], "..") == 0)
        {
            unused_section[i] = 1;
            j = i - 1;
            while (j >= 0 && unused_section[j] == 1)
                j--;
            if (j >= 0)
                unused_section[j] = 1;
        }
        else if (ft_strcmp(path_sections[i], ".") == 0)
            unused_section[i] = 1;
        i++;
    }
    i = 0;
    len = 0;
    while (path_sections[i])
    {
        if (!unused_section[i] && path_sections[i][0])
        {
            len += ft_strlen(path_sections[i]);
            len++;
        }
        i++;
    }
    if (len == 0)
        len++;
    abs_path = (char *)malloc(sizeof(char) * (len + 1));
    if (!abs_path)
        return (free_splited_data(path_sections), NULL);
    i = 0;
    p_dst = abs_path;
    while (path_sections[i])
    {
        if (!unused_section[i] && path_sections[i][0])
        {
            *p_dst++ = '/';
            ft_strcpy(p_dst, path_sections[i]);
            p_dst += ft_strlen(path_sections[i]);
        }
        i++;
    }
    if (p_dst == abs_path)
        *p_dst++ = '/';
    *p_dst = 0;
    free_splited_data(path_sections);
    return (abs_path);
}

int change_to_directory(char *path)
{
    char *abs_path;

    abs_path = make_absolute(path);
    if (chdir(abs_path) < 0)
    {
        return (0);
    }
    return (1);
}

char *concatnate_paths(char *s1, char *s2)
{
    char *tmp;
    char *res;

    tmp = ft_strjoin(s1, "/");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, s2);
    free(tmp);
    return (res);
}

unsigned char builtin_cd(t_tree_node *cmd_node, t_env *env)
{
    char cwd[PATH_MAX];
    char *dirname;
    char *cdpath;
    char **cdpath_arr;
    char *tmp;
    bool printpath;
    size_t i;

    if (!getcwd(cwd, PATH_MAX))
    {
        perror("getcwd(): ");
        return (2);
    }
    printpath = false;
    cdpath = ft_search("CDPATH", env);
    if (!cmd_node->data.command.args[1])
    {
        dirname = ft_search("HOME", env);
        if (!dirname || !dirname[0])
        {
            builtin_error("cd", NULL ,"HOME not set");
            return (2);
        }
    }
    else if (cmd_node->data.command.args[2])
    {
        builtin_error("cd", NULL, "too many arguments");
        return (2);
    }
    else if (ft_strcmp(cmd_node->data.command.args[1], "-") == 0)
    {
        dirname = ft_search("OLDPWD", env);
        printpath = true;
        if (!dirname || !dirname[0])
        {
            builtin_error("cd", NULL, "OLDPWD not set");
            return (2);
        }
    }
    else
    {
        dirname = cmd_node->data.command.args[1];
        if (!absolute_pathname(cmd_node->data.command.args[1]) && cdpath)
        {
            cdpath_arr = ft_split(cdpath, ':');
            i = 0;
            while (cdpath_arr[i])
            {
                tmp = concatnate_paths(cdpath_arr[i], cmd_node->data.command.args[1]);
                if (change_to_directory(tmp))
                {
                    if (cdpath_arr[i][0])
                        printf("%s\n", tmp);
                    free(tmp);
                    return (bindpwd(env));
                }
                else
                    free(tmp);
            }
        }
        if (cmd_node->data.command.args[1][0] != '/')
        {
            tmp = concatnate_paths(cwd, cmd_node->data.command.args[1]);
            if (change_to_directory(tmp))
            {
                free(tmp);
                return (bindpwd(env));
            }
            else
                free(tmp);
        }
    }
    if (change_to_directory(dirname))
    {
        if (printpath)
            printf("%s\n", dirname);
        return (bindpwd(env));
    }
    builtin_error("cd", dirname, strerror(errno));
    return (2);
}  

// void buildin_error(char *cmd_name, char *arg, char *errmsg)
// {
//     ft_putstr_fd("minishell: ", STDERR_FILENO);
//     ft_putstr_fd(cmd_name, STDERR_FILENO);
//     if (arg)
//     {
//         ft_putstr_fd(": ", STDERR_FILENO);
//         ft_putchar_fd(arg, STDERR_FILENO);
//     }
//     ft_putstr_fd(": ", STDERR_FILENO);
//     ft_putstr_fd(errmsg, STDERR_FILENO);
// }