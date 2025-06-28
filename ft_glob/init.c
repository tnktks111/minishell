/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:28 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 12:01:20 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"

//"/Users/Guest" -> 2, "venv/*" -> 2

size_t	_cnt_depth(char *pattern);
t_wildcard_type	_judge_chr_type(char c, bool *in_squote, bool *in_dquote);
t_wildcard_type	*_create_type_arr(char *pattern);
int	*_is_wildcard_splitter(size_t window_size, size_t len, t_wildcard_type *type_array_start);
int	_create_is_wildcard(t_wildcard_type *type_array, t_matching_info *info, char *pattern);
char	*_pattern_section_splitter(char *pattern_start, size_t window_size, size_t len, t_wildcard_type *type_array_start);
int	_create_pattern_sections(t_wildcard_type *type_array, t_matching_info *info, char *pattern);
int	_init_matching_info(t_matching_info *info, char *pattern);

size_t	_cnt_depth(char *pattern)
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

t_wildcard_type	_judge_chr_type(char c, bool *in_squote, bool *in_dquote)
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

t_wildcard_type	*_create_type_arr(char *pattern)
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
		type_array[idx] = _judge_chr_type(pattern[idx], &in_squote, &in_dquote);
		idx++;
	}
	return (type_array);
}

int	*_is_wildcard_splitter(size_t window_size, size_t len, t_wildcard_type *type_array_start)
{
	size_t	idx;
	size_t	sec_idx;
	int	*is_wildcard;

	is_wildcard = (int *)malloc(sizeof(int) * (len));
	if (!is_wildcard)
		return (NULL);
	idx = 0;
	sec_idx = 0;
	while (idx < window_size)
	{
		if (type_array_start[idx] == EXPANDABLE_ASTERISK)
			is_wildcard[sec_idx++] = 1;
		else if (type_array_start[idx] == PLAIN_CHR)
			is_wildcard[sec_idx++] = 0;
		idx++;
	}
	return (is_wildcard);
}

int	_create_is_wildcard(t_wildcard_type *type_array, t_matching_info *info, char *pattern)
{
	size_t	idx;
	size_t	left;
	size_t	right;
	size_t	cnt;

	info->is_wildcard = (int **)malloc(sizeof(int *) * (info->depth + 1));
	if (!info->is_wildcard)
		return (EXIT_FAILURE);
	cnt = 0;
	idx = 0;
	left = 0;
	right = 0;
	while (right < ft_strlen(pattern))
	{
		if (type_array[right] == SLASH)
		{
			info->is_wildcard[idx] = _is_wildcard_splitter(right
					- left, cnt, &type_array[left]);
			if (!info->is_wildcard[idx++])
				return (EXIT_FAILURE);
			while (right < ft_strlen(pattern) && type_array[right] == SLASH)
				right++;
			left = right;
			cnt = 0;
		}
		cnt += (type_array[right] == PLAIN_CHR
				|| type_array[right] == EXPANDABLE_ASTERISK);
		right++;
	}
	if (left < right)
	{
		info->is_wildcard[idx] = _is_wildcard_splitter(right
				- left, cnt, &type_array[left]);
		if (!info->is_wildcard[idx++])
			return (EXIT_FAILURE);
	}
	return (info->is_wildcard[idx] = NULL, EXIT_SUCCESS);
}

char	*_pattern_section_splitter(char *pattern_start, size_t window_size, size_t len, t_wildcard_type *type_array_start)
{
	size_t		idx;
	size_t		sec_idx;
	char		*section;

	section = (char *)malloc(sizeof(char) * (len + 1));
	if (!section)
		return (NULL);
	section[len] = 0;
	idx = 0;
	sec_idx = 0;
	while (idx < window_size)
	{
		if (type_array_start[idx] != QUOTE)
			section[sec_idx++] = pattern_start[idx];
		idx++;
	}
	return (section);
}

int	_create_pattern_sections(t_wildcard_type *type_array, t_matching_info *info, char *pattern)
{
	size_t	idx;
	size_t	left;
	size_t	right;
	size_t	cnt;

	info->pattern_sections = (char **)malloc(sizeof(char *) * (info->depth
				+ 1));
	if (!info->pattern_sections)
		return (EXIT_FAILURE);
	cnt = 0;
	idx = 0;
	left = 0;
	right = 0;
	while (right < ft_strlen(pattern))
	{
		if (type_array[right] == SLASH)
		{
			info->pattern_sections[idx] = _pattern_section_splitter(pattern
					+ left, right - left, cnt, &type_array[left]);
			if (!info->pattern_sections[idx++])
				return (EXIT_FAILURE);
			while (right < ft_strlen(pattern) && type_array[right] == SLASH)
				right++;
			left = right;
			cnt = 0;
		}
		cnt += (type_array[right] == PLAIN_CHR
				|| type_array[right] == EXPANDABLE_ASTERISK);
		right++;
	}
	if (left < right)
	{
		info->pattern_sections[idx] = _pattern_section_splitter(pattern + left,
				right - left, cnt, &type_array[left]);
		if (!info->pattern_sections[idx++])
			return (EXIT_FAILURE);
	}
	return (info->pattern_sections[idx] = NULL, EXIT_SUCCESS);
}

int	_init_matching_info(t_matching_info *info, char *pattern)
{
	t_wildcard_type	*type_array;

	info->total_cnt = 0;
	info->res = NULL;
	info->error_happened = false;
	info->head_slash_cnt = cnt_head_slashes(pattern);
	info->contain_tail_slash = judge_contain_tail_slash(pattern);
	if (pattern[0] == '/')
	{
		info->is_abs_path = true;
		pattern++;
	}
	else
		info->is_abs_path = false;
	while (*pattern == '/')
		pattern++;
	info->depth = _cnt_depth(pattern);
	type_array = _create_type_arr(pattern);
	if (!type_array)
		return (EXIT_FAILURE);
	if (_create_is_wildcard(type_array, info, pattern) == EXIT_FAILURE)
		return (free(type_array), EXIT_FAILURE);
	if (_create_pattern_sections(type_array, info, pattern) == EXIT_FAILURE)
		return (free(type_array), EXIT_FAILURE);
	return (free(type_array), EXIT_SUCCESS);
}
