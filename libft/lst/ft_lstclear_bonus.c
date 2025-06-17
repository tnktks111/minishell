/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:11:56 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/30 16:37:30 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cur;
	t_list	*tmp;

	if (!lst || !(*lst) || !(del))
		return ;
	cur = *lst;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		del(tmp->content);
		free(tmp);
	}
	*lst = NULL;
}
// compile with ft_lstnew_bonus.c, ft_lstadd_back_bonus.c
// int main()
// {
// 	t_list *l = ft_lstnew(malloc(1));
// 	for (int i = 0; i < 10; ++i)
// 		ft_lstadd_back(&l, ft_lstnew(malloc(1)));
// 	ft_lstclear(&l, free);
// 	if (l)
// 	{
// 		printf("Error\n");
// 		return (1);
// 	}
// 	printf("Success\n");
// }