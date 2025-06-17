/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:25:00 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/03 11:51:50 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

int	ft_toupper(int c)
{
	if (ft_islower(c))
		c -= 32;
	return (c);
}

// if the given parameter is not an unsigned char val(0 ~ 255),
//	the behavior is undefined.
// int main()
// {
// 	for (int c = 0; c <= 255; ++c)
// 	{
// 		if (ft_toupper(c) != toupper(c))
// 			printf("at %d, ft: %d, original: %d\n", c, ft_toupper(c),
//				toupper(c));
// 	}
// }
