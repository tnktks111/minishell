/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:35:45 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 19:02:21 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_env_count(char const *s);
static char	*env_word_splitter(char const *s);
static char	**free_and_fail(char **words, size_t allocated_count);
static char	**ft_env_split(char const *s);
void here_doc_expander(char **s, t_env *env);

static size_t	ft_env_count(char const *s)
{
	size_t	cnt;
    bool in_var;

	if (!s || !s[0])
		return (0);
    cnt = (s[0] != '$');
    in_var = (s[0] == '$');
	while (*s)
	{
        if (*s == '$')
        {
            if (*(s+1) && (ft_isdigit(*(s+1)) || *(s+1) == '?'))
            {
                s++;
                if (*(s+2) && *(s+2) == '$')
                    cnt++;
                in_var = false;

            }
            else
                in_var = true;
            cnt++;
        }
        else if (in_var == true && !(ft_isalnum(*s) || *s == '_'))
        {
            in_var = false;
            cnt++;
        }
        s++;
	}
	return (cnt);
}

static char	*env_word_splitter(char const *s)
{
	size_t	i;
	char	*res;

    if (!s || !s[0])
        return (NULL);
    i = 0;
    if (s[0] == '$')
    {
        if (s[1] && (s[1] == '?' || ft_isdigit(s[1])))
        {
            res = ft_strndup(s, 2);
            return (res);
        }
        i++;
        while(s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
            i++;
        res = ft_strndup(s, i);
        return (res);
    }
    else
    {
        while (s[i] && s[i] != '$')
            i++;
        res = ft_strndup(s, i);
        return (res);
    }
}

static char	**free_and_fail(char **words, size_t allocated_count)
{  
	size_t	i;

	i = 0;
	while (i < allocated_count)
		free(words[i++]);
	free(words);
	return (NULL);
}

static char	**ft_env_split(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**words;

	len = ft_env_count(s);
	words = (char **)malloc(sizeof(char *) * (len + 1));
	if (!words)
		return (NULL);
	words[len] = NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		words[i] = env_word_splitter(s + j);
		if (!words[i])
			return (free_and_fail(words, i + 1));
        j += ft_strlen(words[i]);
        i++;
	}
	
	return (words);
}

void here_doc_expander(char **s, t_env *env)
{
	char *res;
	char **tokens;
	char *newtoken;
	size_t i;

	tokens = ft_env_split(*s);
	// if (!tokens)
	// {
		
	// }
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{			
			if (tokens[i][1])
			{
				newtoken = ft_strdup(ft_search(&tokens[i][1], env));
				free(tokens[i]);
				tokens[i] = newtoken;
			}
		}
		i++;
	}
	res = ft_concatenate_strarr(tokens);
	// if (!res)
	// {
		
	// }
	free_splited_data(tokens);
	free(*s);
	*s = res;
}

