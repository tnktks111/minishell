/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:29 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 17:24:01 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	if (!dst || !src)
		return (0);
	i = 0;
	j = 0;
	while (dst[i])
		i++;
	if (dstsize > i)
	{
		while (j < dstsize - i - 1 && src[j])
		{
			dst[i + j] = src[j];
			j++;
		}
		dst[i + j] = '\0';
		while (src[j])
			j++;
	}
	j = ft_strlen(src);
	if (i >= dstsize)
		return (dstsize + j);
	else
		return (i + j);
}

// //compile with ft_strlen.c
// int main() {

//     char dest[20];

//     strcpy(dest, "abc");
//     printf("%zu\n", ft_strlcat(dest, "def", sizeof(dest)));
//     printf("%s\n", dest);

//     strcpy(dest, "abc");
//     printf("%zu\n", ft_strlcat(dest, "defgh", 8));
//     printf("%s\n", dest);

//     dest[0] = '\0';
//     printf("%zu\n", ft_strlcat(dest, "xyz", 20));
//     printf("%s\n", dest);

//     strcpy(dest, "abc");
//     printf("%zu\n", ft_strlcat(dest, "", 20));
//     printf("%s\n", dest);

//     dest[0] = '\0';
//     printf("%zu\n", ft_strlcat(dest, "abc", 1));
//     printf("%s\n", dest);

//     dest[0] = '\0';
//     printf("%zu\n", ft_strlcat(dest, "abc", 0));
//     printf("%s\n", dest);

//     strcpy(dest, "abcdef");
//     printf("%zu\n", ft_strlcat(dest, "ghi", 6));
//     printf("%s\n", dest);

//     strcpy(dest, "abc");
//     printf("%zu\n", ft_strlcat(dest, "de", 6));
//     printf("%s\n", dest);

//     strcpy(dest, "abcd");
//     printf("%zu\n", ft_strlcat(dest, "efg", 6));
//     printf("%s\n", dest);

//     strcpy(dest, "abcde");
//     printf("%zu\n", ft_strlcat(dest, "f", 6));
//     printf("%s\n", dest);

//     return 0;
// }