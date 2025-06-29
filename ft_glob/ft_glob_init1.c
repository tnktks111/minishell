/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_init1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:28 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/29 16:54:26 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//"/Users/Guest" -> 2, "venv/*" -> 2

int			*_is_wc_splitter(size_t window_size, size_t len,
				t_wildcard_type *type_array_start);
char		*_ptn_splitter(char *pattern_start, size_t window_size, size_t len,
				t_wildcard_type *type_array_start);
int			_init_pattern_and_wc_secs(t_wildcard_type *type_array,
				t_matching_info *info, char *pattern);
int			_init_matching_info(t_matching_info *info, char *pattern);

int	*_is_wc_splitter(size_t window_size, size_t len,
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

char	*_ptn_splitter(char *pattern_start, size_t window_size, size_t len,
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

static int	_set_split_sections(t_matching_info *info, t_wildcard_type *arr,
		char *p, t_split_vars *v)
{
	info->ptn_secs[v->idx] = _ptn_splitter(p + v->l, v->r - v->l, v->cnt,
			&arr[v->l]);
	info->is_wildcard[v->idx] = _is_wc_splitter(v->r - v->l, v->cnt,
			&arr[v->l]);
	if (!info->ptn_secs[v->idx] || !info->is_wildcard[v->idx])
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	exec_init_pt_wc_loop(t_matching_info *info, t_wildcard_type *arr,
		char *p, size_t len)
{
	t_split_vars	v;

	ft_memset(&v, 0, sizeof(t_split_vars));
	while (v.r < len)
	{
		if (arr[v.r] == SLASH)
		{
			if (_set_split_sections(info, arr, p, &v) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			while (v.r < len && arr[v.r] == SLASH)
				v.r++;
			v.l = v.r;
			v.cnt = 0;
			v.idx++;
		}
		else
			v.cnt += is_valid_pattern_chr(arr[v.r++]);
	}
	if (v.l < v.r)
	{
		if (_set_split_sections(info, arr, p, &v) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	_init_pattern_and_wc_secs(t_wildcard_type *type_array,
		t_matching_info *info, char *pattern)
{
	info->ptn_secs = (char **)ft_calloc(info->depth + 1, sizeof(char *));
	if (!info->ptn_secs)
		return (EXIT_FAILURE);
	info->is_wildcard = (int **)ft_calloc(info->depth + 1, sizeof(int *));
	if (!info->is_wildcard)
		return (free(info->ptn_secs), EXIT_FAILURE);\
	if (exec_init_pt_wc_loop(info, type_array, pattern,
			ft_strlen(pattern)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
