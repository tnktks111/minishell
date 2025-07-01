/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_append_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:48:54 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:48:54 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*new_token(char *start, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strdup_len(start, len);
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

size_t	append_param_token_and_move_index(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 1;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strncmp(str, "(", 1) == 0)
		new->status = LEFT_PAREN;
	else if (ft_strncmp(str, ")", 1) == 0)
		new->status = RIGHT_PAREN;
	append_token(head, new);
	return (len);
}

size_t	append_token_and_move_index(t_token **head, char *str, bool d_quote,
		bool s_quote)
{
	size_t	len;
	t_token	*new;

	if (d_quote)
		len = in_d_quote_token_strlen(str);
	else if (s_quote)
		len = in_s_quote_token_strlen(str);
	else
		len = token_strlen(str);
	new = new_token(str, len);
	if (!new)
		return (0);
	if (d_quote)
		new->status = IN_DOUBLE;
	else if (s_quote)
		new->status = IN_SINGLE;
	else
		new->status = NORMAL;
	append_token(head, new);
	return (len);
}

size_t	append_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 1;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strcmp(new->str, "|") == 0)
		new->status = PIPE;
	else if (ft_strcmp(new->str, "<") == 0)
		new->status = REDIRECT;
	else if (ft_strcmp(new->str, ">") == 0)
		new->status = REDIRECT;
	else
		new->status = SPLITABLE;
	append_token(head, new);
	return (len);
}

size_t	append_two_word_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 2;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strncmp(new->str, "||", 3) == 0)
		new->status = AND_OR;
	else if (ft_strncmp(new->str, "&&", 3) == 0)
		new->status = AND_OR;
	else if (ft_strncmp(new->str, "<<", 3) == 0)
		new->status = REDIRECT;
	else if (ft_strncmp(new->str, ">>", 3) == 0)
		new->status = REDIRECT;
	else
		new->status = SPLITABLE;
	append_token(head, new);
	return (len);
}
