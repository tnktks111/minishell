/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:09 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 10:49:06 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

static int	ft_isspace(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	return (uc == ' ' || (9 <= uc && uc <= 13));
}

static int	is_overflow(int sign, unsigned long num, unsigned long x)
{
	unsigned long	cutoff;

	if (sign > 0)
		cutoff = LONG_MAX;
	else
		cutoff = (unsigned long)LONG_MAX + 1;
	return (cutoff / 10 < num || cutoff - num * 10 < x);
}

int	ft_atoi(const char *str)
{
	unsigned long	num;
	int				sign;

	if (!str)
		return (0);
	num = 0;
	sign = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str && (*str == '+' || *str == '-'))
	{
		if (*str++ == '-')
			sign *= -1;
	}
	while (*str && ('0' <= *str && *str <= '9'))
	{
		if (is_overflow(sign, num, *str - '0'))
		{
			if (sign > 0)
				return ((int)LONG_MAX);
			return ((int)LONG_MIN);
		}
		num = num * 10 + (*str++ - '0');
	}
	return ((int)(sign * num));
}

int	ft_atoi_for_shlvl(const char *str)
{
	unsigned long	num;
	int				sign;

	if (!str)
		return (0);
	num = 0;
	sign = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str && (*str == '+' || *str == '-'))
	{
		if (*str++ == '-')
			sign *= -1;
	}
	while (*str && ft_isdigit(*str))
	{
		num = num * 10 + (*str++ - '0');
		if ((sign == 1 && num > INT_MAX) || (sign == -1 && num > 2147483648L))
			return (INT_MAX);
	}
	if (*str)
		return (0);
	return ((int)(sign * num));
}

// #include <stdio.h>
// int main()
// {
//     char *s = "9223372036854775811";
//     printf("%d\n", ft_atoi(s));
// 	printf("%d\n", atoi(s));
// }
