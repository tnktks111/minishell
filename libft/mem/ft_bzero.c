/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 21:14:18 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*c;

	if (!s)
		return ;
	c = (unsigned char *)s;
	while (n)
	{
		*c++ = '\0';
		n--;
	}
}

// int main()
// {
// 	char tab[100];
// 	memset(tab, 'A', 100);
// 	ft_bzero(tab, 0);
// 	if (tab[0] != 'A')
// 	{
// 		printf("Error_1\n");
// 		return(1);
// 	}
// 	ft_bzero(tab, 42);
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
// 	ft_bzero(NULL, 42);
// 	printf("Success\n");
// 	return (0);
// }
