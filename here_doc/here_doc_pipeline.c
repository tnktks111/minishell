/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:07:11 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 10:11:51 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	prepare_pl_here_docs(t_tree_node *node_pipeline, t_env *env)
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
		if (prepare_here_doc(curr, env) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		curr = curr->parent;
	}
	return (EXIT_SUCCESS);
}
