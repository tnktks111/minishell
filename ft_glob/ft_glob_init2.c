/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob_init2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:39:24 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 21:56:32 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
