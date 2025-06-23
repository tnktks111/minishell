/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:32:10 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:32:10 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redirect_kind	get_redirect_kind(t_token *redirect_token)
{
	char	*redirect_str;

	redirect_str = redirect_token->str;
	if (ft_strcmp(redirect_str, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(redirect_str, "<<") == 0)
		return (REDIR_HEREDOC);
	else if (ft_strcmp(redirect_str, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(redirect_str, ">") == 0)
		return (REDIR_OUT);
	else
		return (REDIR_IN);
}

int	get_io_number(t_redirect_kind kind, t_token *redirect_token)
{
	t_token	*prev;

	prev = redirect_token->prev;
	if (!prev || prev->status != NORMAL)
	{
		if (kind == REDIR_IN || kind == REDIR_HEREDOC)
			return (0);
		else if (kind == REDIR_OUT || kind == REDIR_APPEND)
			return (1);
		else
			return (1);
	}
	prev->status = USED;
	return (ft_atoi(prev->str));
}

char	*get_output_name(t_token *redirect_token)
{
	t_token	*cur;
	char	*file_name;

	cur = redirect_token->next;
	while (cur && cur->status == SPLITABLE)
		cur = cur->next;
	cur->status = USED;
	file_name = ft_strdup(cur->str);
	return (file_name);
}

bool	check_if_expandable(char *filename)
{
	size_t	len;

	if (!filename)
		return (false);
	len = ft_strlen(filename);
	if (len >= 2 && is_s_quote(filename[0]) && is_s_quote(filename[len - 1]))
		return (false);
	return (ft_strchr(filename, '$') != NULL);
}
