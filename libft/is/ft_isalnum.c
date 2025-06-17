/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:29 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 14:58:55 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return ((ft_isalpha(c) || ft_isdigit(c)));
}

// // compile with ft_isalpha.c and ft_isalnum.c
// int main()
// {
// 	for (int i = 0; i < 255; i++)
// 	{
// 		if (!(isalnum(i)) != !(ft_isalnum(i)))
// 			printf("Fail at %d", i);
// 	}
// }
