/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-29 06:18:48 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-29 06:18:48 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_status_paren(t_status status)
{
	if (status == RIGHT_PAREN || status == LEFT_PAREN)
		return (true);
	else
		return (false);
}

bool	check_cosecutive_paren_syntax_error(t_token *head)
{
	t_token	*cur;
	t_token	*next;

	cur = head;
	while (cur)
	{
		if (cur->status == RIGHT_PAREN)
		{
			next = cur->next;
			while (next && next->status == SPLITABLE)
				next = next->next;
			if (next && next->status == LEFT_PAREN)
			{
				error_unexpected_token(next->str);
				return (true);
			}
		}
		cur = cur->next;
	}
	return (false);
}

bool	check_closed_paren_syntax_error(t_token *head)
{
	t_token	*cur;
	t_token	*next;

	cur = head;
	while (cur)
	{
		if (cur->status == LEFT_PAREN)
		{
			next = cur->next;
			while (next && next->status == SPLITABLE)
				next = next->next;
			if (next && next->status == RIGHT_PAREN)
			{
				error_unexpected_token(next->str);
				return (true);
			}
		}
		cur = cur->next;
	}
	return (check_cosecutive_paren_syntax_error(head));
}

bool	check_syntax_error(t_token *head)
{
	t_token	*cur;

	cur = head;
	skip_splitable_forward(cur);
	if (is_status_meta(cur->status))
		return (error_unexpected_token(cur->str), true);
	return (check_op_placement(head));
}

bool	check_redirect_placement(t_token *head)
{
	bool	prev_is_redir;
	t_token	*cur;

	prev_is_redir = false;
	cur = head;
	if (!cur)
		return (false);
	while (cur)
	{
		if (cur->status == REDIRECT)
		{
			if (prev_is_redir)
				return (error_unexpected_token(cur->str), true);
			prev_is_redir = true;
		}
		if (is_status_meta(cur->status) || is_status_paren(cur->status))
			if (prev_is_redir)
				return (error_unexpected_token(cur->str), true);
		if (!is_status_splitable(cur->status) && (cur->status != REDIRECT))
			prev_is_redir = false;
		cur = cur->next;
	}
	if (prev_is_redir)
		return (error_unexpected_token("newline"), true);
	return (check_paren_syntax_error(head));
}
