/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:28 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 18:41:39 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//"/Users/Guest" -> 2, "venv/*" -> 2

int		*_is_wildcard_splitter(size_t window_size, size_t len,
			t_wildcard_type *type_array_start);
char	*_ptn_sec_splitter(char *pattern_start, size_t window_size, size_t len,
			t_wildcard_type *type_array_start);
int		_init_pattern_and_wc_secs(t_wildcard_type *type_array,
			t_matching_info *info, char *pattern);
int		_init_matching_info(t_matching_info *info, char *pattern);

int	*_is_wildcard_splitter(size_t window_size, size_t len,
		t_wildcard_type *type_array_start)
{
	size_t	idx;
	size_t	sec_idx;
	int		*is_wildcard;

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

char	*_ptn_sec_splitter(char *pattern_start, size_t window_size, size_t len,
		t_wildcard_type *type_array_start)
{
	size_t	idx;
	size_t	sec_idx;
	char	*section;

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

static void exec_init_pt_wc_loop(t_matching_info *info, t_wildcard_type *type_array, char *pattern, size_t len)
{
	size_t l;
	size_t r;
	size_t idx;
	size_t cnt;

	l = 0;
	r = 0;
	idx = 0;
	cnt = 0;
	while (r < len)
	{
		if (pattern[r] == SLASH)
		{
			info->ptn_secs[idx] = _ptn_sec_splitter(pattern + l, r - l, cnt,
					&type_array[l]);
			info->is_wildcard[idx] = _is_wildcard_splitter(r - l, cnt,
					&type_array[l]);
			if (!info->ptn_secs[idx] || !info->is_wildcard[idx])
				return (EXIT_FAILURE);
			while (r < len && type_array[r] == SLASH)
				r++;
			l = r;
			cnt = 0;
			idx++;
		}
		cnt += (type_array[r] == PLAIN_CHR
				|| type_array[r] == EXPANDABLE_ASTERISK);
		r++;
	}
	if (l < r)
	{
		info->ptn_secs[idx] = _ptn_sec_splitter(pattern + l, r - l, cnt,
				&type_array[l]);
		info->is_wildcard[idx] = _is_wildcard_splitter(r - l, cnt,
				&type_array[l]);
		if (!info->ptn_secs[idx] || !info->is_wildcard[idx])
				return (EXIT_FAILURE);
	}
}

int	_init_pattern_and_wc_secs(t_wildcard_type *type_array,
		t_matching_info *info, char *pattern)
{
	info->ptn_secs = (char **)ft_calloc(info->depth + 1, sizeof(char *));
	if (!info->ptn_secs)
		return (EXIT_FAILURE);
	info->is_wildcard = (int **)ft_malloc(info->depth + 1, sizeof(int *));
	if (!info->is_wildcard)
		return (free(info->ptn_secs), EXIT_FAILURE);

	return (EXIT_SUCCESS);
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
	info->depth = cnt_depth(pattern);
	type_array = create_type_arr(pattern);
	if (!type_array)
		return (EXIT_FAILURE);
	if (_init_pattern_and_wc_secs(type_array, info, pattern) == EXIT_FAILURE)
		return (free(type_array), EXIT_FAILURE);
	return (free(type_array), EXIT_SUCCESS);
}
