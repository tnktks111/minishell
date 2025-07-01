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

#include "../inc/minishell.h"

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

bool	find_bang(t_token *head, t_token *tail)
{
	t_token	*cur;
	bool	result;

	cur = NULL;
	result = false;
	if (head)
		cur = head;
	while (cur && tail && cur != tail->next)
	{
		cur = skip_splitable_forward(cur);
		if (!cur)
			break ;
		if (cur && cur->status == LEFT_PAREN)
			break ;
		if (cur->str && ft_strncmp(cur->str, "!", 2) == 0)
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
