/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:26 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 17:28:32 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	newstr = malloc(s1_len + s2_len + 1);
	if (!newstr)
		return (NULL);
	ft_strlcpy(newstr, s1, s1_len + 1);
	ft_strlcpy(&newstr[s1_len], s2, s2_len + 1);
	return (newstr);
}

// //compile with ft_strlen.c ft_strlcpy.c
// int main()
// {
// 	printf("%s\n", ft_strjoin(NULL, NULL));
// 	char s1[10] = "42";
// 	char s2[10] = "Tokyo";
// 	printf("%s", ft_strjoin(s1, s2));
// }