/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:18 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 17:06:30 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//find c in s
char	*ft_strchr(const char *s, int c)
{
	char	*p;
	char	c_cpy;

	if (!s)
		return (NULL);
	p = (char *)s;
	c_cpy = (char)c;
	while (*p && *p != c_cpy)
		p++;
	if (*p == '\0' && c_cpy != '\0')
		p = NULL;
	return (p);
}

// int main()
// {
//     char s[] = "Hello World";
// 	printf("%s\n", ft_strchr(NULL, '\0'));
//     printf("%s\n",ft_strchr(s, 'H' + 256));
// }
