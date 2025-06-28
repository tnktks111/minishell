/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:33 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 11:51:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"

bool			judge_contain_tail_slash(char *pattern);
char			*join_path(char *dir, char *file);
t_wildcard_tree	*_create_wildcard_tree_node(char *parent_path, char *filename);
size_t			_cnt_base_dir_file(char *base_dir, bool show_hidden_files);
t_wildcard_tree	**_gen_base_dir_file_array(char *base_dir,
					bool show_hidden_files);

bool	judge_contain_tail_slash(char *pattern)
{
	while (*pattern)
		pattern++;
	pattern--;
	return (*pattern == '/');
}

char	*join_path(char *dir, char *file)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, file);
	free(tmp);
	return (res);
}

t_wildcard_tree	*_create_wildcard_tree_node(char *parent_path, char *filename)
{
	t_wildcard_tree	*newnode;
	char			*content;

	content = join_path(parent_path, filename);
	if (!content)
		return (NULL);
	newnode = (t_wildcard_tree *)malloc(sizeof(t_wildcard_tree));
	if (!newnode)
		return (free(content), NULL);
	newnode->filename = ft_strdup(filename);
	if (!newnode->filename)
		return (free(content), free(newnode), NULL);
	newnode->parent = NULL;
	newnode->children = NULL;
	newnode->path = content;
	return (newnode);
}

size_t	_cnt_base_dir_file(char *base_dir, bool show_hidden_files)
{
	DIR				*dir;
	struct dirent	*dp;
	size_t			cnt;

	cnt = 0;
	dir = opendir(base_dir);
	if (dir == NULL)
		return (0);
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (show_hidden_files)
		{
			if (dp->d_name[0] == '.')
				cnt++;
		}
		else
		{
			if (dp->d_name[0] != '.')
				cnt++;
		}
		dp = readdir(dir);
	}
	closedir(dir);
	return (cnt);
}

t_wildcard_tree	**_gen_base_dir_file_array(char *base_dir,
		bool show_hidden_files)
{
	DIR *dir;
	struct dirent *dp;
	size_t cnt;
	t_wildcard_tree **children;

	cnt = _cnt_base_dir_file(base_dir, show_hidden_files);
	children = (t_wildcard_tree **)malloc(sizeof(t_wildcard_tree *) * (cnt
				+ 1));
	if (!children)
		return (NULL);
	children[cnt] = NULL;
	dir = opendir(base_dir);
	if (dir == NULL)
		return (free(children), (NULL));
	cnt = 0;
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (show_hidden_files && dp->d_name[0] == '.')
		{
			children[cnt] = _create_wildcard_tree_node(base_dir, dp->d_name);
			if (!children[cnt])
				return (NULL);
			cnt++;
		}
		if (!show_hidden_files && dp->d_name[0] != '.')
		{
			children[cnt] = _create_wildcard_tree_node(base_dir, dp->d_name);
			if (!children[cnt])
				return (NULL);
			cnt++;
		}
		dp = readdir(dir);
	}
	closedir(dir);
	return (children);
}