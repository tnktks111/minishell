/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:43:59 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/16 18:44:22 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	_cnt_tmp_dir_file(void)
{
	DIR				*dir;
	struct dirent	*dp;
	size_t			cnt;

	cnt = 0;
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (dp->d_name[0] != '.')
			cnt++;
		dp = readdir(dir);
	}
	closedir(dir);
	return (cnt);
}

char	**gen_tmp_dir_file_array(void)
{
	DIR				*dir;
	struct dirent	*dp;
	size_t			cnt;
	char			**res;

	cnt = _cnt_tmp_dir_file();
	res = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (!res)
		return (NULL);
	res[cnt] = NULL;
	dir = opendir(".");
	if (dir == NULL)
		return (free(res), NULL);
	cnt = 0;
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (dp->d_name[0] != '.')
		{
			res[cnt] = ft_strdup(dp->d_name);
			if (!res[cnt])
				return (free_allocated_data(res, cnt), NULL);
			cnt++;
		}
		dp = readdir(dir);
	}
	closedir(dir);
	return (res);
}
