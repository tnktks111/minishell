/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:07:11 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 13:13:35 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	unlink_for_error(t_tree_node *node);
static int	prepare_pl_here_docs(t_tree_node *node_pipeline, t_env *env);
static void	unlink_prev_pls(t_tree_node *node_andor);
int			prepare_entire_here_docs(t_tree_node *root, t_env *env);
void		unlink_all_tmpfiles(t_tree_node *node);

static void	unlink_for_error(t_tree_node *node)
{
	t_tree_node	*curr;

	curr = node;
	while (curr->kind == NODE_PIPE)
	{
		unlink_tmpfile(curr->right);
		curr = curr->left;
	}
	unlink_tmpfile(curr);
}

static int	prepare_pl_here_docs(t_tree_node *node_pipeline, t_env *env)
{
	t_tree_node	*curr;

	curr = node_pipeline->left;
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	if (prepare_here_doc(curr, env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	curr = curr->parent;
	while (curr->kind == NODE_PIPE)
	{
		if (prepare_here_doc(curr->right, env) == EXIT_FAILURE)
		{
			unlink_for_error(curr);
			return (EXIT_FAILURE);
		}
		curr = curr->parent;
	}
	return (EXIT_SUCCESS);
}

static void	unlink_prev_pls(t_tree_node *node_andor)
{
	t_tree_node	*curr;

	curr = node_andor;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
	{
		unlink_pl_tmpfiles(curr->right);
		curr = curr->left;
	}
	unlink_pl_tmpfiles(curr);
}

int	prepare_entire_here_docs(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;

	curr = root->left;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
		curr = curr->left;
	if (prepare_pl_here_docs(curr, env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	curr = curr->parent;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
	{
		if (prepare_pl_here_docs(curr->right, env) == EXIT_FAILURE)
			return (unlink_prev_pls(curr->left), EXIT_FAILURE);
		curr = curr->parent;
	}
	return (EXIT_SUCCESS);
}

void	unlink_all_tmpfiles(t_tree_node *node)
{
	if (!node)
		return ;
	if (node->kind == NODE_SIMPLE_COMMAND || node->kind == NODE_SUBSHELL)
		unlink_tmpfile(node);
	unlink_all_tmpfiles(node->left);
	unlink_all_tmpfiles(node->right);
}
