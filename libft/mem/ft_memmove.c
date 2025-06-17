/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:55 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/01 19:20:10 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_cpy;
	unsigned char	*src_cpy;
	size_t			i;

	if (!dst || !src)
		return (dst);
	dst_cpy = (unsigned char *)dst;
	src_cpy = (unsigned char *)src;
	if (!len || dst == src)
		return (dst);
	if (dst_cpy < src_cpy)
	{
		i = 0;
		while (i < len)
			dst_cpy[i++] = *src_cpy++;
	}
	else
	{
		while (len > 0)
		{
			len--;
			dst_cpy[len] = src_cpy[len];
		}
	}
	return (dst);
}

// int main()
// {
// 	char tab[100];
// 	char src[100];
// 	memset(tab, 'A', 100);
// 	memset(src, 0, 100);
// 	ft_memmove(tab, src, 0);
// 	if (tab[0] != 'A')
// 	{
// 		printf("Error_1\n");
// 		return(1);
// 	}
// 	ft_memmove(tab, src, 42);
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
// 	if (ft_memmove(NULL, src, 42))
// 	{
// 		printf("Error_4\n");
// 		return (1);
// 	}

// 	char tab2[20] = "aaaa42tokyo";
// 	char ans[20] = "42tokyookyo";
// 	ft_memmove(tab2, tab2 + 4, 7);
// 	if (strcmp(tab2, ans))
// 	{
// 		printf("Error_5\n");
// 		return(1);
// 	}
// 	printf("Success\n");
// 	return (0);
// }
