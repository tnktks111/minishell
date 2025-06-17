/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:24:24 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 17:34:42 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (*s)
		(*f)(i++, s++);
}

// void plus_idx(unsigned int i, char *s)
// {
// 	*s += i;
// }

// int main(void)
// {
// 	ft_striteri(NULL, plus_idx);
// 	char s1[] = "";
// 	ft_striteri(s1, plus_idx);
// 	if (strcmp(s1, ""))
// 		printf("Error_1\n");

// 	char s2[] = "0";
// 	ft_striteri(s2, plus_idx);
// 	if (strcmp(s2, "0"))
// 		printf("Error_2\n");

// 	char s3[] = "000000000";
// 	ft_striteri(s3, plus_idx);
// 	if (strcmp(s3, "012345678"))
// 		printf("Error_3\n");
// 	printf("Success\n");
// }