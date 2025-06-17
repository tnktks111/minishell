/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:45 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 19:02:10 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack)
		return (NULL);
	if (!(*needle))
		return ((char *)(haystack));
	while (i < len && haystack[i])
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (i + j < len && haystack[i + j]
				&& needle[j] && haystack[i + j] == needle[j])
				j++;
			if (!needle[j])
				return ((char *)(&haystack[i]));
		}
		i++;
	}
	return (NULL);
}

// int main()
// {
// 	if (ft_strnstr(NULL, NULL, 100))
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	char haystack[] = "qwertyui42tokopasdfghjkl42tokyowertyuiasdfgh";
// 	char needle[] = "42tokyo";
// 	printf("%s\n", ft_strnstr(haystack, needle, 100));
// }
