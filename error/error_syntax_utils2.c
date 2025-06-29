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
	return (false);
}
