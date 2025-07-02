/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable_utils3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-02 07:12:38 by sguruge           #+#    #+#             */
/*   Updated: 2025-07-02 07:12:38 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static size_t	ft_count_isspace(char const *s)
{
	size_t	cnt;

	cnt = 0;
	while (*s)
	{
		while (*s && is_space(*s))
			s++;
		if (*s)
		{
			cnt++;
			while (*s && !is_space(*s))
				s++;
		}
	}
	return (cnt);
}

static char	*word_splitter_isspace(char const *s)
{
	size_t	i;
	char	*word;

	i = 0;
	while (s[i] && !is_space(s[i]))
		i++;
	word = malloc(i + 1);
	if (!word)
		return (NULL);
	word[i] = '\0';
	i = 0;
	while (s[i] && !is_space(s[i]))
	{
		word[i] = s[i];
		i++;
	}
	return (word);
}

static char	**free_and_fail(char **words, size_t allocated_count)
{
	size_t	i;

	i = 0;
	while (i < allocated_count)
	{
		free(words[i]);
		i++;
	}
	free(words);
	return (NULL);
}

char	**ft_split_isspace(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**words;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	len = ft_count_isspace(s);
	words = (char **)malloc(sizeof(char *) * (len + 1));
	if (!words)
		return (NULL);
	while (i < len)
	{
		while (s[j] && is_space(s[j]))
			j++;
		words[i] = word_splitter_isspace(s + j);
		if (!words[i])
			return (free_and_fail(words, i));
		while (s[j] && !is_space(s[j]))
			j++;
		i++;
	}
	return (words[len] = NULL, words);
}
