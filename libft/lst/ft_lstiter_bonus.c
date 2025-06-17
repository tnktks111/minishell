/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:19:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/27 18:29:16 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!f)
		return ;
	while (lst)
	{
		(*f)(lst->content);
		lst = lst->next;
	}
}

// //compile with ft_lstnew_bonus.c, ft_lstadd_back_bonus.c
// void	plus_one(void *content)
// {
// 	if (!content)
// 		return;
// 	*(char *)content += 1;
// }

// int main()
// {
// 	t_list *l = NULL;
// 	t_list *cur;
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		char *c_ptr = malloc(sizeof(char));
// 		*c_ptr = '0';
// 		ft_lstadd_back(&l, ft_lstnew(c_ptr));
// 	}
// 	ft_lstiter(l, plus_one);
// 	cur = l;
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		printf("%c\n", *(char *)(cur->content));
// 		cur = cur->next;
// 	}
// }