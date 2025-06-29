/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:33:59 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:33:59 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_node_kind	get_node_kind(t_token *token)
{
	if (ft_strncmp(token->str, "||", 3) == 0)
		return (NODE_OR);
	else if (ft_strncmp(token->str, "&&", 3) == 0)
		return (NODE_AND);
	else if (ft_strncmp(token->str, "|", 2) == 0)
		return (NODE_PIPE);
	else
		return (NODE_SIMPLE_COMMAND);
}

t_token	*find_third_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;
	int		level;

	if (!head || !tail)
		return (NULL);
	cur = tail;
	level = 0;
	while (cur && cur != head->prev)
	{
		if (cur == head->prev)
			break ;
		if (cur->status == LEFT_PAREN)
			level--;
		else if (cur->status == RIGHT_PAREN)
			level++;
		else if (level == 0 && (cur->status == PIPE))
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

t_token	*find_second_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_AND)
			return (cur);
		cur = cur->prev;
	}
	return (find_third_lowest_precedence_operator(head, tail));
}

t_token	*find_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_OR)
			return (cur);
		cur = cur->prev;
	}
	return (find_second_lowest_precedence_operator(head, tail));
}

t_token	*find_logical_operator(t_token *head, t_token *tail)
{
	t_token	*cur;
	int		level;

	if (!head || !tail)
		return (NULL);
	cur = tail;
	level = 0;
	while (cur && cur != head->prev)
	{
		if (cur == head->prev)
			break ;
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
