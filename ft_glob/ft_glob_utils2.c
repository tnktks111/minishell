/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 20:18:47 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool			is_hidden(char *filename);
char			*create_n_slashes(size_t n);
size_t			cnt_head_slashes(char *pattern);
size_t			cnt_depth(char *pattern);
t_wildcard_type	judge_chr_type(char c, bool *in_squote, bool *in_dquote);

bool	is_hidden(char *filename)
{
	if (!filename)
		return (false);
	return (filename[0] == '.');
}

char	*create_n_slashes(size_t n)
{
	char	*res;

	res = (char *)malloc(sizeof(char) * (n + 1));
	if (!res)
		return (res);
	ft_memset(res, '/', n);
	res[n] = 0;
	return (res);
}

size_t	cnt_head_slashes(char *pattern)
{
	size_t	cnt;

	cnt = 0;
	while (*pattern && *pattern == '/')
	{
		cnt++;
		pattern++;
	}
	return (cnt);
}

size_t	cnt_depth(char *pattern)
{
	size_t	cnt;

	cnt = 0;
	while (*pattern)
	{
		if (*pattern != '/')
		{
			cnt++;
			while (*pattern && *pattern != '/')
				pattern++;
		}
		else
			pattern++;
	}
	return (cnt);
}

t_wildcard_type	judge_chr_type(char c, bool *in_squote, bool *in_dquote)
{
	if (c == '/')
		return (SLASH);
	if (*in_squote == true && c == '\'')
		return (*in_squote = false, QUOTE);
	if (*in_dquote == true && c == '\"')
		return (*in_dquote = false, QUOTE);
	if (*in_squote == false && *in_dquote == false)
	{
		if (c == '\'')
			return (*in_squote = true, QUOTE);
		if (c == '\"')
			return (*in_dquote = true, QUOTE);
		if (c == '*')
			return (EXPANDABLE_ASTERISK);
	}
	return (PLAIN_CHR);
}
