/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:25 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 11:24:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			_set_child_of_wc_tree_node(t_wc_tree *node, bool show_hidden_files);
static int	_build_wc_tree_helper(t_wc_tree *node, t_matching_info *info);
void		_build_wc_tree_recursive(t_wc_tree *node, size_t depth,
				t_matching_info *info);
t_wc_tree	*_gen_root_node(bool is_abs_path, size_t head_slash_cnt);
int			ft_glob(char *pattern, t_list **res_head);

int	_set_child_of_wc_tree_node(t_wc_tree *node, bool show_hidden_files)
{
	size_t	i;

	node->children = _gen_base_dir_file_array(node->path, show_hidden_files);
	if (!node->children)
		return (EXIT_FAILURE);
	i = 0;
	while (node->children[i])
		node->children[i++]->parent = node;
	return (EXIT_SUCCESS);
}

static int	_build_wc_tree_helper(t_wc_tree *node, t_matching_info *info)
{
	char	*content;
	t_list	*newnode;

	content = adjust_result(node->path, info->is_abs_path,
			info->contain_tail_slash);
	if (!content)
	{
		info->error_happened = true;
		return (EXIT_FAILURE);
	}
	newnode = ft_lstnew(content);
	if (!newnode)
	{
		info->error_happened = true;
		return (EXIT_FAILURE);
	}
	info->total_cnt++;
	ft_lstadd_back(&info->res, newnode);
	return (EXIT_SUCCESS);
}

void	_build_wc_tree_recursive(t_wc_tree *node, size_t depth,
		t_matching_info *info)
{
	size_t	i;
	bool	show_hidden_files;

	if (!node || !info || info->error_happened)
		return ;
	if (!info->ptn_secs || !info->ptn_secs)
	{
		info->error_happened = true;
		return ;
	}
	if (depth == info->depth)
	{
		_build_wc_tree_helper(node, info);
		return ;
	}
	show_hidden_files = (info->ptn_secs[depth][0] == '.');
	_set_child_of_wc_tree_node(node, show_hidden_files);
	i = 0;
	while (node->children && node->children[i])
	{
		if (ft_ismatch(node->children[i]->filename, info->ptn_secs[depth],
				info->is_wildcard[depth], ft_strlen(info->ptn_secs[depth])))
			_build_wc_tree_recursive(node->children[i], depth + 1, info);
		i++;
	}
}

t_wc_tree	*_gen_root_node(bool is_abs_path, size_t head_slash_cnt)
{
	t_wc_tree	*root;

	root = (t_wc_tree *)malloc(sizeof(t_wc_tree));
	if (!root)
		return (NULL);
	if (is_abs_path)
		root->path = create_n_slashes(head_slash_cnt);
	else
		root->path = ft_strdup(".");
	if (!root->path)
		return (free(root), NULL);
	root->parent = NULL;
	root->children = NULL;
	root->filename = NULL;
	return (root);
}

int	ft_glob(char *pattern, t_list **res_head)
{
	t_matching_info	info;
	t_wc_tree		*root;

	_init_matching_info(&info, pattern);
	root = _gen_root_node(info.is_abs_path, info.head_slash_cnt);
	if (!root)
		return (-1);
	_build_wc_tree_recursive(root, 0, &info);
	destroy_entire_tree_recursive(root);
	free_splitted_data(info.ptn_secs);
	free_int_matrix(info.is_wildcard);
	*res_head = info.res;
	return (info.total_cnt);
}
