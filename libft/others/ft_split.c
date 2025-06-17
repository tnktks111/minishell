/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 18:58:29 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	ft_count(char const *s, char c)
{
	size_t	cnt;

	if (!s || !s[0])
		return (0);
	cnt = 0;
	while (*s)
	{
		if (*s != c)
		{
			cnt++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (cnt);
}

static char	*word_splitter(char const *s, char c)
{
	size_t	i;
	char	*word;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	word = malloc(i + 1);
	if (!word)
		return (NULL);
	word[i] = '\0';
	i = 0;
	while (s[i] && s[i] != c)
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
		free(words[i++]);
	free(words);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**words;

	len = ft_count(s, c);
	words = (char **)malloc(sizeof(char *) * (len + 1));
	if (!words)
		return (NULL);
	words[len] = NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		while (s[j] && s[j] == c)
			j++;
		words[i] = word_splitter(s + j, c);
		if (!words[i++])
			return (free_and_fail(words, i));
		while (s[j] && s[j] != c)
			j++;
	}
	return (words);
}

// int main()
// {
// 	char ***tests = (char ***)malloc(sizeof(char **) * 9);
//     char **words = ft_split("aaaaaaa", 'a');
//     printf("%s\n", words[0]);
// 	tests[0] = ft_split("Hello World", ' ');
// 	tests[1] = ft_split("Hello                             World", ' ');
// 	tests[2] = ft_split("       Leading and trailing spaces       ", ' ');
// 	tests[3] = ft_split("            ", ' ');
// 	tests[4] = ft_split("JustOneWord", ' ');
// 	tests[5] = ft_split("", ' ');
// 	tests[6] = ft_split(NULL, ' ');
// 	tests[7] = ft_split("Split\0by\0null", '\0');
// 	int j;
// 	for (int i = 0; i < 8; i++)
// 	{
// 		printf("-test%d-\n", i);
// 		j = 0;
// 		while(tests[i][j])
// 		{
// 			printf("%d: %s\n", j + 1, tests[i][j]);
// 			j++;
// 		}
// 		printf("\n");
// 	}
// }