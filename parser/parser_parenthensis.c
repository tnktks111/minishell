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

#include "../inc/minishell.h"

t_token	*get_redir_tail(t_token *head, t_token *tail)
{
	t_token	*cur;
	t_token	*last;
	bool	found_redir;

	found_redir = false;
	cur = head;
	last = head;
	while (cur)
	{
		if (cur->status == REDIRECT)
			found_redir = !found_redir;
		else if (found_redir && (cur->status == NORMAL
				|| cur->status == IN_DOUBLE || cur->status == IN_SINGLE))
		{
			last = cur;
			return (last);
		}
		cur = cur->next;
	}
	return (tail);
}

t_tree_node	*parse_paren(t_create_tree *tree, t_token *head, t_token *tail)
{
	t_token	*paren_tail;

	paren_tail = find_matching_paren(head);
	if (paren_tail)
	{
		tree->pipeline_root = create_tree(head->next, paren_tail->prev);
		tree->paratheneses_root = create_subshell_node(tree->pipeline_root,
				head, paren_tail);
		return (create_pipeline_node(tree->paratheneses_root, head, tail));
	}
	return (NULL);
}
