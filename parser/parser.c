/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 09:35:10 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/23 14:55:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree_node	*add_tree_root(t_tree_node *root)
{
	t_tree_node	*new_root;

	new_root = malloc(sizeof(t_tree_node));
	if (!new_root)
		return (NULL);
	new_root->kind = NODE_ROOT;
	new_root->parent = NULL;
	new_root->left = root;
	new_root->right = NULL;
	new_root->data.pipeline.have_bang = false;
	new_root->data.pipeline.exit_status = 0;
	if (root)
		root->parent = new_root;
	return (new_root);
}

t_tree_node	*create_subshell_node(t_tree_node *cur_root)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_SUBSHELL;
	node->left = cur_root;
	node->right = NULL;
	node->parent = NULL;
	if (cur_root)
		cur_root->parent = node;
	return (node);
}

t_tree_node	*create_operator_node(t_token *op, t_tree_node *left,
		t_tree_node *right)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = get_node_kind(op);
	node->parent = NULL;
	node->left = left;
	node->right = right;
	if (left)
		left->parent = node;
	if (right)
		right->parent = node;
	return (node);
}

t_tree_node	*create_tree(t_token *head, t_token *tail)
{
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;
	t_tree_node	*pipeline_root;
	t_tree_node	*paratheneses_root;

	if (!head || !tail)
		return (NULL);
	head = skip_splitable_forward(head);
	tail = skip_splitable_backward(tail);
	if (is_parentheses_group(head, tail))
	{
		pipeline_root = create_tree(head->next, tail->prev);
		paratheneses_root = create_subshell_node(pipeline_root);
		return (create_pipeline_node(paratheneses_root, head, tail));
	}
	op = find_logical_operator(head, tail);
	if (!op)
	{
		pipeline_root = create_pipeline_tree(head, tail);
		return (create_pipeline_node(pipeline_root, head, tail));
	}
	left = create_tree(head, op->prev);
	right = create_tree(op->next, tail);
	return (create_operator_node(op, left, right));
}

t_tree_node	*parser(t_token *head, t_env *env)
{
	t_tree_node	*root;
	t_token		*tail;
	bool		syntax_error;

	root = NULL;
	syntax_error = check_syntax_error(head);
	if (syntax_error)
		handle_syntax_error(env);
	else
	{
		tail = get_tail(head);
		root = create_tree(head, tail);
		root = add_tree_root(root);
		free_token(head, tail);
		print_tree(root);
		exec_ast(root, env);
		print_tree(root);
	}
	return (root);
}
