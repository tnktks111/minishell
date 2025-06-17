/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:50 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/01 18:59:28 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	*init_seen(char const *s1, int *seen, char const *set)
{
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (i < 256)
		seen[i++] = 0;
	i = 0;
	while (set[i])
		seen[(unsigned char)set[i++]] = 1;
	return (seen);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		seen[256];
	size_t	start;
	size_t	end;

	if (!(init_seen(s1, seen, set)))
		return (NULL);
	start = 0;
	while (s1[start] && seen[(unsigned char)s1[start]])
		start++;
	end = ft_strlen(s1);
	while (end > 0 && seen[(unsigned char)s1[end - 1]])
		end--;
	return (ft_substr(s1, start, end - start));
}

// // complie with ft_strlen
// int main()
// {
// 	char s1[] = "aaaabbbbbabaa42tokyoaaaaa42ataobbbakayaoaaaaaaaaaaaaaaaaaaaa";
// 	char set[] = "aaabbbbb2b";
// 	printf("%s\n", ft_strtrim(s1, set));
// }
