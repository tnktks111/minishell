/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:35:45 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 22:49:25 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_env_count_loop(char const *s, size_t *cnt, bool *in_var);
static size_t	ft_env_count(char const *s);
static char		*env_word_splitter(char const *s);
static char		**ft_env_split(char const *s);
int				here_doc_expander(char **s, t_env *env);

static void	ft_env_count_loop(char const *s, size_t *cnt, bool *in_var)
{
	while (*s)
	{
		if (*s == '$')
		{
			if (*(s + 1) && (ft_isdigit(*(s + 1)) || *(s + 1) == '?'))
			{
				s++;
				(*cnt) += (*(s + 2) && *(s + 2) == '$');
				*in_var = false;
			}
			else
				*in_var = true;
			(*cnt)++;
		}
		else if (*in_var == true && !(ft_isalnum(*s) || *s == '_'))
		{
			*in_var = false;
			(*cnt)++;
		}
		s++;
	}
}

static size_t	ft_env_count(char const *s)
{
	size_t	cnt;
	bool	in_var;

	if (!s || !s[0])
		return (0);
	cnt = (s[0] != '$');
	in_var = (s[0] == '$');
	ft_env_count_loop(s, &cnt, &in_var);
	return (cnt);
}

static char	*env_word_splitter(char const *s)
{
	size_t	i;

	if (!s || !s[0])
		return (NULL);
	i = 0;
	if (s[0] == '$')
	{
		if (s[1] && (s[1] == '?' || ft_isdigit(s[1])))
			return (ft_strndup(s, 2));
		i++;
		while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
			i++;
		return (ft_strndup(s, i));
	}
	else
	{
		while (s[i] && s[i] != '$')
			i++;
		return (ft_strndup(s, i));
	}
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
			return (free_allocated_data(words, i));
		j += ft_strlen(words[i++]);
	}
	return (words);
}

int	here_doc_expander(char **s, t_env *env)
{
	char	*res;
	char	**tokens;
	char	*newtoken;
	size_t	i;

	tokens = ft_env_split(*s);
	if (!tokens)
		return (EXIT_FAILURE);
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '$' && tokens[i][1])
		{
			newtoken = ft_search(&tokens[i][1], env);
			free(tokens[i]);
			tokens[i] = newtoken;
		}
		i++;
	}
	res = ft_concatenate_strarr(tokens);
	if (!res)
		return (free_splited_data(tokens), free(*s), EXIT_FAILURE);
	free(*s);
	*s = res;
	return (free_splited_data(tokens), EXIT_SUCCESS);
}
