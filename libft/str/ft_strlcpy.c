/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:32 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/26 20:33:12 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	res;

	if (!dst || !src)
		return (0);
	i = 0;
	res = ft_strlen(src);
	if (dstsize == 0)
		return (res);
	while (i < dstsize - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (res);
}

// int main()
// {
// 	char dst[] = "42 Tokyo";
// 	char src[] = "aiueo";
// 	printf("%zu\n", ft_strlcpy(dst, src, 10));
// 	printf("%s\n", dst);
// }
