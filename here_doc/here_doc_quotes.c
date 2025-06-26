/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:34:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/26 20:31:45 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

bool have_quotes(char *limiter);
static int judge_type(char c, bool *in_squote, bool *in_d_quote);
static char *gen_quote_removed_str(char *limiter, int *is_removable, size_t len);
static int *gen_is_removable(char *limiter, size_t *size);
void remove_quotes(t_redirect *redirect);

bool have_quotes(char *limiter)
{
	size_t i;

	i = 0;
	while (limiter[i])
	{
		if (limiter[i] == '\"' || limiter[i] == '\'')
			return true;
		i++;
	}
	return false;
}

static int judge_type(char c, bool *in_squote, bool *in_d_quote)
{
	if (*in_squote == false && *in_d_quote == false)
	{
		if (c == '\'')
			return (*in_squote = true, 0);
		else if (c == '\"')
			return (*in_d_quote = true, 0);
		else
			return (1);
	}
	else if (*in_squote == true)
	{
		if (c == '\'')
			return (*in_squote = false, 0);
		else
			return (1);
	}
	else
	{
		if (c == '\"')
			return (*in_d_quote = false, 0);
		else
			return (1);
	}
}

static char *gen_quote_removed_str(char *limiter, int *is_removable, size_t len)
{
	size_t i;
	size_t j;
	char *newlimiter;
	newlimiter = malloc(sizeof(char) * (len + 1));
	if (!newlimiter)
		return (NULL);
	i = 0;
	j = 0;
	while (limiter[i])
	{
		if (is_removable[i])
			newlimiter[j++] = limiter[i];
		i++;
	}
	newlimiter[j] = 0;
	return (newlimiter);
}

static int *gen_is_removable(char *limiter, size_t *size)
{
	int *is_removable;
	bool in_squote;
	bool in_dquote;
	size_t i;

	is_removable = (int *)malloc(sizeof(int) * ft_strlen(limiter));
	if (!is_removable)
		return (NULL);
	in_squote = false;
	in_dquote = false;
	i = 0;
	while (limiter[i])
	{
		is_removable[i] = judge_type(limiter[i], &in_squote, &in_dquote);
		*size += is_removable[i++];
	}
	return (is_removable);
}

void remove_quotes(t_redirect *redirect)
{
	char *limiter;
	int *is_removable;
	size_t len;
	
	len = 0;
	limiter = redirect->filename;
	is_removable = gen_is_removable(limiter, &len);
	if (!is_removable)
		return;
	redirect->filename = gen_quote_removed_str(limiter, is_removable, len);
	if (!redirect->filename)
	{
		free(is_removable);
		free(limiter);
		return;
	}
	free(is_removable);
	free(limiter);
}
