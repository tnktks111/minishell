/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:47 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 15:32:52 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*res;
	char	c_cpy;

	if (!s)
		return (NULL);
	res = NULL;
	c_cpy = (char)c;
	while (*s)
	{
		if (*s == c_cpy)
			res = (char *)s;
		s++;
	}
	if (c_cpy == '\0')
		res = (char *)s;
	return (res);
}

// #include <stdio.h>
// int main()
// {
//     char *s = "Hello World";
//     printf("%s", ft_strrchr(s, 0));
// }
