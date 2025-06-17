/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:23:32 by ttanaka           #+#    #+#             */
/*   Updated: 2025/05/03 11:42:30 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

static int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_isalpha(int c)
{
	return (ft_islower(c) || ft_isupper(c));
}

// int main()
// {
// 	for (int i = 0; i < 1000; i++)
// 	{
// 		if (!(isalpha(i)) != !(ft_isalpha(i)))
// 			printf("Fail at %d\n%d,%d\n", i, isalpha(i), ft_isalpha(i));
// 	}
// }
