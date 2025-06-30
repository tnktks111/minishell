/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:46:02 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:46:02 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_input(char *input, t_env *env)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		if (!is_space(input[i]))
			return (check_for_unclosed_quotes(input, env));
		i++;
	}
	return (false);
}

bool	is_two_word_splitable(char c1, char c2)
{
	if (c1 == '<' && c2 == '<')
		return (true);
	else if (c1 == '>' && c2 == '>')
		return (true);
	else if (c1 == '|' && c2 == '|')
		return (true);
	else if (c1 == '&' && c2 == '&')
		return (true);
	else
		return (false);
}

char	*ft_strdup_len(char *src, size_t len)
{
	char	*res;
	size_t	i;

	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

size_t	token_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && !is_splitable(str[len]) && !is_param(str[len])
		&& !is_d_quote(str[len]) && !is_s_quote(str[len]) && !(str[len + 1]
			&& is_two_word_splitable(str[len], str[len + 1])))
		len++;
	return (len);
}

bool	check_for_unclosed_quotes(char *input, t_env *env)
{
	bool	in_squote;
	bool	in_dquote;
	char	temp;
	size_t	i;

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (input[i])
	{
		if (!in_dquote && is_s_quote(input[i]))
		{
			temp = input[i];
			in_squote = !in_squote;
		}
		if (!in_squote && is_d_quote(input[i]))
		{
			temp = input[i];
			in_dquote = !in_dquote;
		}
		i++;
	}
	if (!in_dquote && !in_squote)
		return (true);
	return (handle_syntax_error(env), error_unexpected_token(&temp), false);
}
