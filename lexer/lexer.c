/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/29 18:14:20 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*tokenize_str(char *str)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] && str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
			i += append_two_word_splitable(&head, &str[i]);
		else if (is_param(str[i]))
			i += append_param_token_and_move_index(&head, &str[i]);
		else if (is_d_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], true, false);
		else if (is_s_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], false, true);
		else if (is_splitable(str[i]))
			i += append_splitable(&head, &str[i]);
		else
			i += append_token_and_move_index(&head, &str[i], false, false);
	}
	return (head);
}

t_token	*lexer(char *str)
{
	t_token	*head;

	head = tokenize_str(str);
	fix_non_splitted_tokens(&head);
	return (head);
}
