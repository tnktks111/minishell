/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:34:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 19:34:37 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

bool have_quotes(char *limiter);
void remove_quotes(char **limiter);

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

void remove_quotes(char **limiter)
{
	char *newlimiter;
	size_t i;
	size_t j;
	size_t quote_cnt;

	i = 0;
	quote_cnt = 0;
	while ((*limiter)[i])
	{
		if ((*limiter)[i] == '\"' || (*limiter)[i] == '\'')
			quote_cnt++;
		i++;
	}
	newlimiter = malloc(sizeof(char) * (i - quote_cnt + 1));
	if (!newlimiter)
	{
		free(*limiter);
		*limiter = NULL;
		return ;
	}
	i = 0;
	j = 0;
	while ((*limiter)[i])
	{
		if ((*limiter)[i] != '\'' && (*limiter)[i] != '\"')
			newlimiter[j++] = (*limiter)[i];
		i++;
	}
	newlimiter[j] = 0;
	free(*limiter);
	*limiter = newlimiter;
}