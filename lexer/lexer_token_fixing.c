/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fixing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-28 07:06:16 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-28 07:06:16 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	get_status(t_status status1, t_status status2)
{
	if (status1 != NORMAL || status2 != NORMAL)
		return (IN_DOUBLE);
	return (NORMAL);
}

void	join_consecutive_tokens(t_token **cur)
{
	t_token	*first;
	t_token	*second;
	t_token	*next_token;
	t_token	*joined;

	first = *cur;
	second = first->next;
	next_token = second->next;
	joined = malloc(sizeof(t_token));
	if (!joined)
		return ;
	joined->str = ft_strjoin(first->str, second->str);
	if (!joined->str)
		return (free(joined));
	joined->status = get_status(first->status, second->status);
	joined->next = next_token;
	joined->prev = first->prev;
	if (first->prev)
		first->prev->next = joined;
	if (next_token)
		next_token->prev = joined;
	*cur = joined;
	return (free(first->str), free(second->str), free(first), free(second));
}

bool	is_mergeable(t_status status)
{
	return (status == IN_SINGLE || status == IN_DOUBLE || status == NORMAL);
}

void	fix_non_splitted_tokens(t_token **head)
{
	t_token	**cur;

	cur = head;
	while (cur && (*cur)->next)
	{
		if (is_mergeable((*cur)->status) && is_mergeable((*cur)->next->status))
		{
			join_consecutive_tokens(cur);
		}
		else
			cur = &(*cur)->next;
	}
}
