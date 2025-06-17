/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:58:11 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 19:03:08 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cur_last;

	if (!lst)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	cur_last = *lst;
	while (cur_last && cur_last->next)
		cur_last = cur_last->next;
	cur_last->next = new;
}

// compile with ft_lstnew_bonus.c
// int main()
// {
// 	ft_lstadd_back(NULL, ft_lstnew((void*)1));
// 	t_list *l1 = NULL;
// 	ft_lstadd_back(&l1, ft_lstnew((void*)1));
// 	if (l1->content != (void*)1 || l1->next)
// 		printf("Fail\n");

// 	ft_lstadd_back(&l1, ft_lstnew((void*)2));
// 	if (l1->content != (void*)1 || (l1->next)->content != (void*)2
//		|| (l1->next)->next)
// 		printf("Fail\n");
// }