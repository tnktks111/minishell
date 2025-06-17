/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:24 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 14:30:46 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	bytes;
	void	*block;

	bytes = count * size;
	if (count && size != bytes / count)
		return (NULL);
	block = malloc(bytes);
	if (!block)
		return (NULL);
	ft_memset(block, 0, bytes);
	return (block);
}

// #include <stdlib.h>
// #include <stdio.h>
// int main()
// {
//     // int INT_MIN = -2147483648;
//     printf("%p\n", ft_calloc(-5, 0));
//     printf("%p", calloc(-5, 0));
// }
