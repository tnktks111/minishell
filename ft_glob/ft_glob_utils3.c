/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:50:55 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 20:18:51 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_wildcard_type	*create_type_arr(char *pattern);
bool			is_valid_pattern_chr(t_wildcard_type type);

t_wildcard_type	*create_type_arr(char *pattern)
{
	t_wildcard_type	*type_array;
	size_t			idx;
	bool			in_squote;
	bool			in_dquote;

	in_squote = false;
	in_dquote = false;
	type_array = (t_wildcard_type *)malloc(sizeof(t_wildcard_type)
			* ft_strlen(pattern));
	if (!type_array)
		return (NULL);
	idx = 0;
	while (pattern[idx])
	{
		type_array[idx] = judge_chr_type(pattern[idx], &in_squote, &in_dquote);
		idx++;
	}
	return (type_array);
}

bool	is_valid_pattern_chr(t_wildcard_type type)
{
	return (type == EXPANDABLE_ASTERISK || type == PLAIN_CHR);
}
