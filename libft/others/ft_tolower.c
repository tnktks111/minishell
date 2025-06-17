/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:57 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/03 11:52:00 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_tolower(int c)
{
	if (ft_isupper(c))
		c += 32;
	return (c);
}

// // if the given parameter is not an unsigned char val(0 ~ 255),
// // the behavior is undefined.
// int main()
// {
// 	for (int c = 0; c <= 255; ++c)
// 	{
// 		if (ft_tolower(c) != tolower(c))
// 			printf("at %d, ft: %d, original: %d\n", c, ft_tolower(c),
//				tolower(c));
// 	}
// }
