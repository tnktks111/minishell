/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 06:05:37 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/30 18:34:13 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_status_meta(t_status status)
{
	if (status == AND_OR || status == PIPE)
		return (true);
	else
		return (false);
}

bool	is_status_splitable(t_status status)
{
	if (status == SPLITABLE)
		return (true);
	else
		return (false);
}

void	handle_syntax_error(t_env *env)
{
	env->prev_exit_status = 2;
}

bool	check_paren_syntax_error(t_token *head)
{
	t_token	*cur;
	int		level;
	char	*temp;

	level = 0;
	cur = head;
	while (cur)
	{
		if (cur->status == LEFT_PAREN)
		{
			temp = cur->str;
			level++;
		}
		else if (cur->status == RIGHT_PAREN)
		{
			temp = cur->str;
			level--;
			if (level < 0)
				return (error_unexpected_token(cur->str), true);
		}
		cur = cur->next;
	}
	if (level != 0)
		return (error_unexpected_token(temp), true);
	return (check_closed_paren_syntax_error(head));
}

bool	check_op_placement(t_token *head)
{
	bool	prev_is_meta;
	char	*temp;
	t_token	*cur;

	prev_is_meta = false;
	cur = head;
	if (!cur)
		return (false);
	while (cur)
	{
		if (is_status_meta(cur->status))
		{
			if (prev_is_meta)
				return (error_unexpected_token(temp), true);
			temp = cur->str;
			prev_is_meta = true;
		}
		if (!is_status_splitable(cur->status) && !is_status_meta(cur->status))
			prev_is_meta = false;
		cur = cur->next;
	}
	if (prev_is_meta)
		return (error_unexpected_token("newline"), true);
	return (check_redirect_placement(head));
}
