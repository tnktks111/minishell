/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 16:50:56 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_n_slashes(size_t n)
{
	char *res;

	res = (char *)malloc(sizeof(char) *(n + 1));
	if (!res)
		return (res);
	ft_memset(res, '/', n);
	res[n] = 0;
	return (res);
}

size_t	cnt_head_slashes(char *pattern)
{
	size_t cnt;

	cnt = 0;
	while (*pattern && *pattern == '/')
	{
		cnt++;
		pattern++;
	}
	return (cnt);
}
