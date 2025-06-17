/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:21:24 by ttanaka           #+#    #+#             */
/*   Updated: 2025/04/30 17:00:38 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*new_node;
	void	*new_content;

	if (!lst || !f || !del)
		return (NULL);
	head = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			free(new_content);
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, new_node);
		lst = lst->next;
	}
	return (head);
}

// //compile with ft_lstnew_bonus.c, ft_lstadd_back_bonus.c
// void	*plus_one(void *content)
// {
// 	void *res = malloc(sizeof(char));
// 	if (!content)
// 		return (NULL);
// 	*(char *)res = *(char *)(content) + 1;
// 	return (res);
// }

// int main()
// {
// 	t_list *l = NULL;
// 	t_list *cur;
// 	t_list *map;
// 	char *s = "012345678";
// 	for (int i = 0; i < 9; ++i)
// 	{
// 		ft_lstadd_back(&l, ft_lstnew(s + i));
// 	}
// 	map = ft_lstmap(l, &plus_one, free);
// 	cur = l;
// 	printf("-original list-\n");
// 	for (int i = 0; i < 9; ++i)
// 	{
// 		printf("%c\n", *(char *)(cur->content));
// 		cur = cur->next;
// 	}
// 	printf("-created list-\n");
// 	cur = map;
// 	for (int i = 0; i < 9; ++i)
// 	{
// 		printf("%c\n", *(char *)(cur->content));
// 		cur = cur->next;
// 	}
// }
