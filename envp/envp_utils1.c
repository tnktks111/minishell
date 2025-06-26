/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:27:04 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/26 21:06:25 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *extract_key(char *s);
char *extract_val(char *s);
char *extract_key_when_additional(char *s);
char *extract_val_when_additional(char *s, char *key, t_env *env);
char *_concatnate_key_val(char *key, char *val, bool have_quote);

char *extract_key(char *s)
{
    char *tmp;
    char *res;
    char *sep;

    tmp = ft_strdup(s);
    if (!tmp)
        return (NULL);
    sep = ft_strchr(tmp, '=');
	if (!sep)
		return (strdup(s));
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
	char *start;

	start = ft_strchr(s, '=');
	if (!start)
		return (ft_strdup(""));
    res = ft_strdup(start + 1);
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