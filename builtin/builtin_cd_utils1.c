/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:33:04 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 10:40:35 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void		unused_section_init(char **path_sections, int *unused_section);
static size_t	cnt_abs_path_len(char **path_sections, int *unused_section);
static char		*path_compresser(char **path_sections, int *unused_section,
					size_t len);
char			*make_absolute(char *path);

static void	unused_section_init(char **path_sections, int *unused_section)
{
	int	i;
	int	j;

	i = 0;
	while (path_sections[i])
	{
		if (ft_strcmp(path_sections[i], "..") == 0)
		{
			unused_section[i] = 1;
			j = i - 1;
			while (j >= 0 && unused_section[j] == 1)
				j--;
			if (j >= 0)
				unused_section[j] = 1;
		}
		else if (ft_strcmp(path_sections[i], ".") == 0)
			unused_section[i] = 1;
		i++;
	}
}

static size_t	cnt_abs_path_len(char **path_sections, int *unused_section)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (path_sections[i])
	{
		if (!unused_section[i] && path_sections[i][0])
		{
			len += ft_strlen(path_sections[i]);
			len++;
		}
		i++;
	}
	if (len == 0)
		len++;
	return (len);
}

static char	*path_compresser(char **path_sections, int *unused_section,
		size_t len)
{
	char	*abs_path;
	char	*p_dst;
	size_t	i;

	abs_path = (char *)malloc(sizeof(char) * (len + 1));
	if (!abs_path)
		return (NULL);
	i = 0;
	p_dst = abs_path;
	while (path_sections[i])
	{
		if (!unused_section[i] && path_sections[i][0])
		{
			*p_dst++ = '/';
			ft_strcpy(p_dst, path_sections[i]);
			p_dst += ft_strlen(path_sections[i]);
		}
		i++;
	}
	if (p_dst == abs_path)
		*p_dst++ = '/';
	*p_dst = 0;
	return (abs_path);
}

char	*make_absolute(char *path)
{
	char	*abs_path;
	char	**path_sections;
	int		unused_section[PATH_MAX];
	size_t	len;

	ft_bzero((void *)unused_section, sizeof(int) * PATH_MAX);
	path_sections = ft_split(path, '/');
	if (!path_sections)
		return (NULL);
	unused_section_init(path_sections, unused_section);
	len = cnt_abs_path_len(path_sections, unused_section);
	abs_path = path_compresser(path_sections, unused_section, len);
	free_splited_data(path_sections);
	return (abs_path);
}
