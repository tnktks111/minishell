/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/01 19:20:14 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p1;
	const unsigned char	*p2;

	p2 = (const unsigned char *)src;
	if (!dest)
		return (NULL);
	if (!src)
		return (dest);
	p1 = (unsigned char *)dest;
	while (n)
	{
		*p1++ = *p2++;
		n--;
	}
	return (dest);
}

// int main()
// {
// 	char tab[100];
// 	char src[100];
// 	memset(tab, 'A', 100);
// 	memset(src, 0, 100);
// 	ft_memcpy(tab, src, 0);
// 	if (tab[0] != 'A')
// 	{
// 		printf("Error_1\n");
// 		return(1);
// 	}
// 	ft_memcpy(tab, src, 42);
// 	for (int i = 0; i < 42; i++)
// 	{
// 		if (tab[i] == 'A')
// 		{
// 			printf("Error_2\n");
// 			return (1);
// 		}
// 	}
// 	if (tab[42] != 'A')
// 	{
// 		printf("Error_3\n");
// 		return (1);
// 	}
// 	if (ft_memcpy(NULL, src, 42))
// 	{
// 		printf("Error_4\n");
// 		return (1);
// 	}
// 	printf("Success\n");
// 	return (0);
// }
