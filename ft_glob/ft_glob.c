/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:25 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 12:03:32 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wildcard.h"

int				_set_child_of_wc_tree_node(t_wildcard_tree *node,
					bool show_hidden_files);
void			_build_wc_tree_recursive(t_wildcard_tree *node, size_t depth,
					t_matching_info *info);
t_wildcard_tree	*_gen_root_node(bool is_abs_path, size_t head_slash_cnt);
int				ft_glob(char *pattern, t_list **res_head);

char *adjust_result(char *result, bool is_abs_path, bool contain_tail_slash)
{
	char *tmp;
	char *adjusted_res;

	if (is_abs_path)
		tmp = ft_strdup(result + 1);
	else
		tmp = ft_strdup(result + 2);
	if (!tmp)
		return (NULL);
	if (contain_tail_slash)
	{
		adjusted_res = ft_strjoin(tmp, "/");
		free(tmp);
	}
	else
		adjusted_res = tmp;
	return (adjusted_res);
}

int	_set_child_of_wc_tree_node(t_wildcard_tree *node, bool show_hidden_files)
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

void	_build_wc_tree_recursive(t_wildcard_tree *node, size_t depth,
		t_matching_info *info)
{
	size_t	i;
	t_list	*newnode;
	bool	show_hidden_files;
	char	*content;

	if (!node || !info || info->error_happened)
		return ;
	if (!info->pattern_sections || !info->pattern_sections)
	{
		info->error_happened = true;
		return ;
	}
	if (depth == info->depth)
	{
		content = adjust_result(node->path, info->is_abs_path, info->contain_tail_slash);
		if (!content)
		{
			info->error_happened = true;
			return ;
		}
		newnode = ft_lstnew(content);
		if (!newnode)
		{
			info->error_happened = true;
			return ;
		}
		info->total_cnt++;
		ft_lstadd_back(&info->res, newnode);
		return ;
	}
	show_hidden_files = (info->pattern_sections[depth][0] == '.');
	_set_child_of_wc_tree_node(node, show_hidden_files);
	i = 0;
	if (node->children)
	{
		while (node->children[i])
		{
			if (ft_ismatch(node->children[i]->filename,
					info->pattern_sections[depth], info->is_wildcard[depth],
					ft_strlen(info->pattern_sections[depth])))
				_build_wc_tree_recursive(node->children[i], depth + 1, info);
			i++;
		}
	}
}

t_wildcard_tree	*_gen_root_node(bool is_abs_path, size_t head_slash_cnt)
{
	t_wildcard_tree	*root;

	root = (t_wildcard_tree *)malloc(sizeof(t_wildcard_tree));
	if (!root)
		return (NULL);
	if (is_abs_path)
		root->path = create_n_slashes(head_slash_cnt);
	else
		root->path = ".";
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
	t_wildcard_tree	*root;

	_init_matching_info(&info, pattern);
	root = _gen_root_node(info.is_abs_path, info.head_slash_cnt);
	if (!root)
		return (-1);
	_build_wc_tree_recursive(root, 0, &info);
	*res_head = info.res;
	return (info.total_cnt);
}

// int main(void)
// {
//     t_list  *head = NULL;
//     t_list  *current;
//     int     match_count;

//     match_count = ft_glob("../*", &head);

//     printf("Total matches: %d\n", match_count);
//     printf("--- Matched Paths ---\n");

//     current = head;
//     while (current)
//     {
//         printf("%s\n", (char *)current->content);
//         current = current->next;
//     }
//     printf("---------------------\n");

//     return (0);
// }
