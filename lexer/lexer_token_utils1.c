/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:47:40 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:47:40 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_token(t_token **head, t_token *new)
{
	t_token	*last;

	if (!*head)
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

size_t	in_d_quote_token_strlen(char *str)
{
	size_t	len;

	len = 0;
	if (is_d_quote(str[len]))
		len++;
	while (str[len] && !is_d_quote(str[len]))
		len++;
	if (is_d_quote(str[len]))
		len++;
	return (len);
}

size_t	in_s_quote_token_strlen(char *str)
{
	size_t	len;

	len = 0;
	if (is_s_quote(str[len]))
		len++;
	while (str[len] && !is_s_quote(str[len]))
		len++;
	if (is_s_quote(str[len]))
		len++;
	return (len);
}


