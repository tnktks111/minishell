/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:45:16 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:45:16 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_param(char c)
{
	return (c == '(' || c == ')');
}

bool	is_d_quote(char c)
{
	return (c == 34);
}

bool	is_s_quote(char c)
{
	return (c == 39);
}

bool	is_splitable(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<'
		|| c == '>');
}

bool	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}
