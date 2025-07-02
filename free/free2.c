/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:29:27 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 11:24:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_int_matrix(int **matrix);
void	destroy_entire_tree_recursive(t_wc_tree *root);
void	**free_wc_allocated_data(t_wc_tree **datas, size_t allocated);
void	free_tree_node(t_tree_node *node);

void	free_int_matrix(int **matrix)
{
	size_t	i;

	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}

void	destroy_entire_tree_recursive(t_wc_tree *root)
{
	size_t	i;

	if (!root)
		return ;
	free(root->path);
	free(root->filename);
	i = 0;
	if (root->children)
	{
		while (root->children[i])
			destroy_entire_tree_recursive(root->children[i++]);
		free(root->children);
	}
	free(root);
}

void	**free_wc_allocated_data(t_wc_tree **datas, size_t allocated)
{
	size_t	i;

	i = 0;
	while (i < allocated)
		free(datas[i++]);
	free(datas);
	return (NULL);
}

void	free_tree_node(t_tree_node *node)
{
	if (!node)
		return ;
	free_tree_node(node->left);
	free_tree_node(node->right);
	if (node->kind == NODE_SIMPLE_COMMAND || node->kind == NODE_SUBSHELL)
	{
		if (node->data.command.redirects)
			free_redirects(node->data.command.redirects);
		if (node->data.command.args)
			free_splitted_data(node->data.command.args);
	}
	free(node);
}
