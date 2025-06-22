/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_to_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:13:33 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 15:29:02 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count(char const *s, char c)
{
	size_t	cnt;

	if (!s || !s[0])
		return (0);
	cnt = 0;
	while (*s)
	{
		if (*s != c)
		{
			cnt++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (cnt);
}

static t_list	*word_splitter(char const *s, char c)
{
    t_list *new_node;
	size_t	i;
	char	*word;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	word = malloc(i + 1);
	if (!word)
		return (NULL);
	word[i] = '\0';
	i = 0;
	while (s[i] && s[i] != c)
	{
		word[i] = s[i];
		i++;
	}
    new_node = ft_lstnew(word);
    if (!new_node)
        return (free(word), NULL);
	return (new_node);
}

//be careful that this function returns DUMMY NODE !!!
t_list	*ft_split_to_lst(char const *s, char c)
{
	size_t	i;
	size_t	len;
    t_list *head;
    t_list *tail;
    t_list *new_node;

	len = ft_count(s, c);
	head = ft_lstnew(NULL);
	if (!head)
		return (NULL);
    tail = head;
	i = 0;
	while (len-- > 0)
	{
		while (s[i] && s[i] == c)
			i++;
		new_node = word_splitter(s + i, c);
        if (!new_node)
            return (ft_lstclear(&head, free), NULL);
        tail->next = new_node;
        tail = tail->next;
		while (s[i] && s[i] != c)
			i++;
	}
	return (head);
}
