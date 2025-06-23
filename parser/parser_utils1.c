/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:34:43 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:34:43 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_parentheses_group(t_token *head, t_token *tail)
{
	int		paren_level;
	t_token	*cur;

	paren_level = 0;
	cur = head;
	if (!head || !tail)
		return (false);
	if (head->status != LEFT_PAREN || tail->status != RIGHT_PAREN)
		return (false);
	while (cur && cur != tail->next)
	{
		if (cur->status == LEFT_PAREN)
			paren_level++;
		else if (cur->status == RIGHT_PAREN)
			paren_level--;
		if (paren_level == 0 && cur != tail)
			return (false);
		cur = cur->next;
	}
	return (paren_level == 0);
}

bool	find_bang(t_token *head, t_token *tail)
{
	t_token	*cur;
	bool	result;

	result = false;
	cur = head;
	while (cur && cur != tail->next)
	{
		cur = skip_splitable_forward(cur);
		if (!cur)
			break ;
		if (cur->status == LEFT_PAREN)
			break ;
		if (ft_strncmp(cur->str, "!", 2) == 0)
		{
			result = !result;
			cur->status = USED;
			cur = cur->next;
		}
		else
			break ;
	}
	return (result);
}

t_token	*get_tail(t_token *head)
{
	t_token	*tail;
	t_token	*last_valid;

	tail = head;
	last_valid = head;
	while (tail)
	{
		if (tail->str && tail->str[0] != '\0')
			last_valid = tail;
		tail = tail->next;
	}
	return (last_valid);
}

t_token	*skip_splitable_forward(t_token *token)
{
	while (token && token->status == SPLITABLE)
		token = token->next;
	return (token);
}

t_token	*skip_splitable_backward(t_token *token)
{
	while (token && token->status == SPLITABLE)
		token = token->prev;
	return (token);
}
