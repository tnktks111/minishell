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

bool	check_closed_paren_synttax_error(t_token *head)
{
	bool	prev_is_meta;
	t_token	*cur;

	prev_is_meta = false;
	cur = head;
	if (!cur)
		return (false);
	while (cur)
	{
		if (is_status_paren(cur->status))
		{
			if (prev_is_meta)
				return (error_unexpected_token(cur->str), true);
			prev_is_meta = true;
		}
		if (!is_status_splitable(cur->status) && !is_status_paren(cur->status))
			prev_is_meta = false;
		cur = cur->next;
	}
	if (prev_is_meta)
		return (error_unexpected_token(cur->str), true);
	return (false);
}
