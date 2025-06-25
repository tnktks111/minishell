/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 09:35:10 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/24 00:39:52 by ttanaka          ###   ########.fr       */
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

t_tree_node	*create_subshell_node(t_tree_node *cur_root, t_token *head,
		t_token *tail)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_SUBSHELL;
	node->data.command.redirects = extract_redirects(head, tail);
	node->data.command.args = extract_args(head, tail);
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

void		print_tokens(t_token *head);

t_token	*find_matching_paren(t_token *head)
{
	int		level;
	t_token	*cur;

	level = 0;
	cur = head;
	if (!head || head->status != LEFT_PAREN)
		return (NULL);
	while (cur)
	{
		if (cur->status == LEFT_PAREN)
			level++;
		else if (cur->status == RIGHT_PAREN)
		{
			level--;
			if (level == 0)
				return (cur);
		}
		cur = cur->next;
	}
	return (NULL);
}

t_token	*find_logical_operator(t_token *head, t_token *tail)
{
	t_token	*cur;
	int		level;

	cur = tail;
	level = 0;
	while (cur && cur != head->prev)
	{
		if (cur->status == LEFT_PAREN)
			level++;
		else if (cur->status == RIGHT_PAREN)
			level--;
		else if (level == 0 && (cur->status == AND_OR))
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

t_tree_node	*create_tree(t_token *head, t_token *tail)
{
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;
	t_tree_node	*pipeline_root;
	t_tree_node	*paratheneses_root;
	t_token		*paren_tail;

	if (!head || !tail)
		return (NULL);
	head = skip_splitable_forward(head);
	tail = skip_splitable_backward(tail);
	paren_tail = NULL;
	op = find_logical_operator(head, tail);
	if (head && head->status == LEFT_PAREN)
	{
		paren_tail = find_matching_paren(head);
		if (paren_tail)
		{
			pipeline_root = create_tree(head->next, paren_tail->prev);
			paratheneses_root = create_subshell_node(pipeline_root, head,
					paren_tail);
			return (create_pipeline_node(paratheneses_root, head, paren_tail));
		}
	}
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
		// print_tree(root);
	}
	return (root);
}
