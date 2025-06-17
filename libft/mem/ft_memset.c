/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:58 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/01 19:20:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			rest;
	unsigned char	*cur;

	if (!s)
		return (NULL);
	rest = n;
	cur = (unsigned char *)s;
	while (rest)
	{
		*cur++ = (unsigned char)c;
		rest--;
	}
	return (s);
}

// #include <stdio.h>
// int main()
// {
// 	if (ft_memset(NULL, '\0', 100))
// 		printf("Error\n");
//     char buf1[16];
//     ft_memset(buf1, '\0', 16);
//     ft_memset(buf1, 'a', 4);
//     printf("%s\n", buf1);
//     char buf2[16];
//     memset(buf2, '\0', 16);
//     memset(buf2, 'a', 4);
//     printf("%s\n", buf2);
// }
