/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:33:15 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/17 14:00:35 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*expansionの処理次第で調整必要*/
void	build_wildcard_flag(int *is_wildcard, char *pattern, size_t len)
{
	size_t	i;
	bool	in_double_quote;
	bool	in_single_quote;

	i = 0;
	while (i < len)
	{
		if (pattern[i] == '\'')
			in_single_quote = !in_single_quote;
		if (pattern[i] == '\"')
			in_double_quote = !in_double_quote;
		if (pattern[i] == '*' && !in_double_quote && !in_single_quote)
			is_wildcard[i] = true;
		else
			is_wildcard[i] = false;
		i++;
	}
}

// int main()
// {
//     char *text = "";
//     char *pattern = "*";
//     int wildcard[1024];
//     size_t len = ft_strlen(pattern);
//     ft_init_int_array(wildcard, 1024);
//     build_wildcard_flag(wildcard, pattern, len);
//     printf("%d", ft_ismatch(text, pattern, wildcard, len));
// }