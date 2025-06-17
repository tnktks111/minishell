/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:42 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 15:03:27 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// count digits of positive num
static int	count_digit(unsigned int n)
{
	if (0 <= n && n <= 9)
		return (1);
	return (1 + count_digit(n / 10));
}

char	*ft_itoa(int n)
{
	int				i;
	int				len;
	char			*numstr;
	unsigned int	un;
	int				is_neg;

	is_neg = (n < 0);
	if (is_neg)
		un = (unsigned int)(-((long)n));
	else
		un = (unsigned int)(n);
	len = count_digit(un);
	numstr = malloc(len + is_neg + 1);
	if (!numstr)
		return (NULL);
	numstr[len + is_neg] = '\0';
	i = len + is_neg - 1;
	while (i >= 0)
	{
		numstr[i--] = '0' + un % 10;
		un /= 10;
	}
	if (is_neg)
		numstr[0] = '-';
	return (numstr);
}

// #include <stdio.h>
// int main()
// {
// 	int n = -1;
// 	printf("%s", ft_itoa(n));
// }
