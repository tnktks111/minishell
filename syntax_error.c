/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-22 07:34:07 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-22 07:34:07 by sguruge          ###   ########.fr       */
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

bool	check_syntax_error(t_token *head)
{
	bool	prev_is_splitable;
	t_token	*cur;
	char	*current_str;

	current_str = NULL;
	prev_is_splitable = false;
	cur = head;
	while (cur)
	{
		if (!prev_is_splitable && is_status_meta(cur->status))
		{
			prev_is_splitable = true;
			current_str = cur->str;
		}
		else if (prev_is_splitable && is_status_meta(cur->status))
		{
			error_unexpected_token(current_str);
			return (true);
		}
		else if (prev_is_splitable && !is_status_meta(cur->status))
			prev_is_splitable = !prev_is_splitable;
		cur = cur->next;
	}
	return (false);
}
