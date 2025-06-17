/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:46 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 15:20:21 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//search c in s up to  bytes
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;
	unsigned char	c_cpy;

	if (!s)
		return (NULL);
	p = (unsigned char *)s;
	c_cpy = (unsigned char)c;
	while (*p != c_cpy && n)
	{
		p++;
		n--;
	}
	if (n == 0)
		return (NULL);
	return ((void *)(p));
}

// int main()
// {
// 	char s[] = "0123456789";
// 	if (ft_memchr(NULL, 0, 0))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	if (ft_memchr(s, '0', 0) != memchr(s, '0', 0))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	if (ft_memchr(s, '0', 1) != memchr(s, '0', 1))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	if (ft_memchr(s, 0, 100) != memchr(s, 0, 100))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	if (ft_memchr(s, '0' + 1024, 100) != memchr(s, '0' + 1024, 100))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	printf("Success\n");
// 	return(0);
// }