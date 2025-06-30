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

t_tree_node	*create_pipeline_node(t_tree_node *left, t_token *head,
		t_token *tail)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_PIPE_LINE;
	node->parent = NULL;
	node->data.pipeline.have_bang = find_bang(head, tail);
	node->left = left;
	node->right = NULL;
	if (left)
		left->parent = node;
	return (node);
}

t_tree_node	*parse_paren_pipeline(t_create_tree *tree, t_token *head)
{
	t_token	*paren_tail;

	paren_tail = find_matching_paren(head);
	if (paren_tail)
	{
		tree->pipeline_root = create_tree(head->next, paren_tail->prev);
		return (create_subshell_node(tree->pipeline_root, head, paren_tail));
	}
	return (NULL);
}

t_tree_node	*create_pipeline_tree(t_token *head, t_token *tail,
		t_create_tree *tree)
{
	t_tree_node	*left;
	t_tree_node	*right;
	t_token		*and_or;
	t_token		*pipe;

	head = skip_splitable_forward(head);
	tail = skip_splitable_backward(tail);
	and_or = find_logical_operator(head, tail);
	pipe = find_third_lowest_precedence_operator(head, tail);
	if (!and_or && !pipe && head && head->status == LEFT_PAREN)
		return (parse_paren_pipeline(tree, head));
	if (!pipe)
		return (create_simple_cmd_node(head, tail));
	left = create_pipeline_tree(head, pipe->prev, tree);
	right = create_pipeline_tree(pipe->next, tail, tree);
	return (create_operator_node(pipe, left, right));
}
