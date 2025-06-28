/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parenthensis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-28 08:19:17 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-28 08:19:17 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree_node	*parse_paren(t_create_tree *tree, t_token *head)
{
	t_token	*paren_tail;

	paren_tail = find_matching_paren(head);
	if (paren_tail)
	{
		tree->pipeline_root = create_tree(head->next, paren_tail->prev);
		tree->paratheneses_root = create_subshell_node(tree->pipeline_root,
				head, paren_tail);
		return (create_pipeline_node(tree->paratheneses_root, head,
				paren_tail));
	}
	return (NULL);
}
