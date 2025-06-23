/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:43:10 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:43:10 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree_node	*create_pipeline_node(t_tree_node *cur_root, t_token *head,
		t_token *tail)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_PIPE_LINE;
	node->parent = NULL;
	node->data.pipeline.have_bang = find_bang(head, tail);
	node->left = cur_root;
	node->right = NULL;
	if (cur_root)
		cur_root->parent = node;
	return (node);
}

t_tree_node	*create_pipeline_tree(t_token *head, t_token *tail)
{
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;

	op = find_third_lowest_precedence_operator(head, tail);
	if (!op)
	{
		return (create_simple_cmd_node(head, tail));
	}
	left = create_pipeline_tree(head, op->prev);
	right = create_pipeline_tree(op->next, tail);
	return (create_operator_node(op, left, right));
}
