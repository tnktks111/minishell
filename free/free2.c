/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:29:27 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 12:03:11 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_int_matrix(int **matrix);
void destroy_entire_tree_recursive(t_wc_tree *root);

void free_int_matrix(int **matrix)
{
	size_t i;
	
	i = 0;
	while (matrix[i])
		free(matrix[i++]);
	free(matrix);
}

void destroy_entire_tree_recursive(t_wc_tree *root)
{
	size_t i;

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
