/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 17:40:03 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_min(size_t s1, size_t s2)
{
	if (s1 < s2)
		return (s1);
	else
		return (s2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*subs;
	size_t	sub_len;

	if (!s)
		return (NULL);
	sub_len = 0;
	if (start < ft_strlen(s))
		sub_len = ft_min(ft_strlen(s) - start, len);
	subs = malloc(sub_len + 1);
	if (!subs)
		return (NULL);
	subs[sub_len] = '\0';
	i = 0;
	while (i < sub_len)
	{
		subs[i] = s[start + i];
		i++;
	}
	return (subs);
}

// // complile with ft_strlen.c
// int main()
// {
// 	char s[] = "0123456789";
// 	char *test1 = ft_substr(s, 0, 10);
// 	printf("%s\n", test1);
// 	char *test2 = ft_substr(s, 0, 0);
// 	printf("%s\n", test2);
// 	char *test3 = ft_substr(s, 0, 1);
// 	printf("%s\n", test3);
// 	char *test4 = ft_substr(s, 0, 2);
// 	printf("%s\n", test4);
// 	char *test5 = ft_substr(s, 9, 2);
// 	printf("%s\n", test5);
// 	char *test6 = ft_substr(s, 0, 1234);
// 	printf("%s\n", test6);
// 	char *test7 = ft_substr(s, 0, SIZE_MAX);
// 	printf("%s\n", test7);
// 	char *test8 = ft_substr(s, 0, 10);
// 	printf("%s\n", test8);
// }
