/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concatenate_strarr.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:49:32 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 12:49:39 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

char	*ft_concatenate_strarr(char **words)
{
	size_t	i;
	size_t	len;
	char	*res;
	char	*p_dst;

	i = 0;
	len = 0;
	while (words[i])
		len += ft_strlen(words[i++]);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	p_dst = res;
	while (words[i])
	{
		ft_strcpy(p_dst, words[i]);
		p_dst += ft_strlen(words[i]);
		i++;
	}
	*p_dst = 0;
	return (res);
}
