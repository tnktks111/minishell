/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:51:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 18:36:48 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*cur;
	int		cnt;

	cnt = 0;
	cur = lst;
	while (cur)
	{
		cnt++;
		cur = cur->next;
	}
	return (cnt);
}

// // compile with ft_lstnew_bonus.c, ft_lstadd_back_bonus.c
// int main()
// {
// 	t_list *l = NULL;
// 	printf("%d\n", ft_lstsize(NULL));
// 	for (int i = 0; i < 10; ++i)
// 		ft_lstadd_back(&l, ft_lstnew((void *)1));
// 	printf("%d\n", ft_lstsize(l));
// }