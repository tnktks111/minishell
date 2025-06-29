/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-29 05:11:38 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-29 05:11:38 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_ambiguous_redirect(char *filename);
void	error_unexpected_token(char *token_str);
bool	is_status_meta(t_status status);
void	handle_syntax_error(t_env *env);
bool	check_syntax_error(t_token *head);

void	error_ambiguous_redirect(char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
}

void	error_unexpected_token(char *token_str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token", STDERR_FILENO);
	ft_putstr_fd(" `", STDERR_FILENO);
	ft_putstr_fd(token_str, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

bool	is_status_meta(t_status status)
{
	if (status == REDIRECT || status == AND_OR || status == PIPE)
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

	level = 0;
	cur = head;
	if (!head)
		return (false);
	while (cur)
	{
		if (cur->status == LEFT_PAREN)
			level++;
		else if (cur->status == RIGHT_PAREN)
		{
			level--;
			if (level < 0)
				return (error_unexpected_token(cur->str), true);
		}
		cur = cur->next;
	}
	if (level != 0)
		return (error_unexpected_token(cur->str), true);
	else
		return (false);
}

bool	check_syntax_error(t_token *head)
{
	bool	prev_is_meta;
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
				return (error_unexpected_token(cur->str), true);
			prev_is_meta = true;
		}
		else
			prev_is_meta = false;
		cur = cur->next;
	}
	if (prev_is_meta)
		return (error_unexpected_token("newline"), true);
	return (check_paren_syntax_error(head));
}
