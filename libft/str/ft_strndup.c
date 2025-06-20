/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:50:38 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/20 16:51:06 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dst;
	size_t	i;
	size_t	len;

	if (!s1)
		return (NULL);
	i = 0;
	len = ft_strlen(s1);
	if (len > n)
		len = n;
	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	dst[len] = '\0';
	while (i < len)
	{
		dst[i] = s1[i];
		i++;
	}
	return (dst);
}
