/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:46:19 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 19:03:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

// // compile with ft_lstnew_bonus.c
// int main()
// {
// 	ft_lstadd_front(NULL, NULL);
// 	t_list *l = NULL;
// 	ft_lstadd_front(&l, ft_lstnew((void*)1));
// 	if (l->content != (void *)1 || l->next)
// 	{
// 		printf("Error_1\n");
// 		return (1);
// 	}
// 	ft_lstadd_front(&l, ft_lstnew((void *)2));
// 	if (l->content != (void *)2 || (l->next)->content != (void *)1
//		|| (l->next)->next)
// 	{
// 		printf("Error_2\n");
// 		return (1);
// 	}
// 	printf("Success");
// }
