/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:54:52 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 18:14:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*tail;

	if (!lst)
		return (NULL);
	tail = lst;
	while (tail->next)
		tail = tail->next;
	return (tail);
}

// compile with ft_lstnew_bonus.c, ft_lstadd_back.c
// int main()
// {
// 	t_list *l = ft_lstnew((void *)1);
// 	t_list *last;
// 	for (int i = 0; i < 10; ++i)
// 		ft_lstadd_back(&l, ft_lstnew((void *)1));
// 	ft_lstadd_back(&l, ft_lstnew((void *)2));
// 	last = ft_lstlast(l);
// 	if (last->content != (void *)2 || last->next)
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	printf("Success\n");
// }