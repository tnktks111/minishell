/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 22:29:18 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 12:09:14 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_getc(t_buf_node *node)
{
	t_buf	*buffer;

	buffer = &(node->buffer);
	if (buffer->bufsiz <= 0)
	{
		ft_bzero(buffer->buf, BUFFER_SIZE);
		buffer->bufsiz = read(node->fd, buffer->buf, BUFFER_SIZE);
		buffer->bufp = buffer->buf;
		if (buffer->bufsiz < 0)
			return (RD_ERR);
		if (buffer->bufsiz == 0)
			return (EOF);
	}
	(buffer->bufsiz)--;
	return ((unsigned char)(*(buffer->bufp)++));
}

int	ft_putc(t_string *str, char c)
{
	char	*newstr;

	if (str->cap == 0)
	{
		str->str = malloc(BUFFER_SIZE);
		if (!(str->str))
			return (MALLOC_ERR);
		ft_bzero(str->str, BUFFER_SIZE);
		str->len = 0;
		str->cap = BUFFER_SIZE;
	}
	else if (str->len >= str->cap)
	{
		newstr = malloc(str->cap * 2);
		if (!newstr)
			return (MALLOC_ERR);
		ft_bzero(newstr, str->cap * 2);
		ft_memmove(newstr, str->str, str->len);
		free(str->str);
		str->str = newstr;
		str->cap *= 2;
	}
	str->str[(str->len)++] = c;
	return (0);
}

t_buf_node	*ft_search_gnl(int fd, t_buf_node *hashtable)
{
	int			hash;
	t_buf_node	*curr;
	t_buf_node	*prev;
	t_buf_node	*newnode;

	hash = fd % HASH_SIZE;
	curr = &hashtable[hash];
	if (curr->fd == -1)
	{
		init_buf_node(curr, fd);
		return (curr);
	}
	prev = curr;
	while (curr != NULL)
	{
		if (curr->fd == fd)
			return (curr);
		prev = curr;
		curr = curr->next;
	}
	newnode = create_buf_node(fd);
	if (!newnode)
		return (NULL);
	prev->next = newnode;
	return (newnode);
}

int	ft_get_and_put_c(t_buf_node *node, t_string *res)
{
	int	c;

	while (1)
	{
		c = ft_getc(node);
		if (c == RD_ERR)
		{
			if (res->str)
				free(res->str);
			return (0);
		}
		if (c == EOF)
			return (1);
		if (ft_putc(res, (char)c) == MALLOC_ERR)
		{
			if (res->str)
				free(res->str);
			return (0);
		}
		if ((char)c == '\n')
			return (1);
	}
}

char	*get_next_line(char *s, int fd)
{
	static t_hashtable	hashtable;
	t_buf_node			*node;
	t_string			res;
	int					i;

	if (!hashtable.initialized)
	{
		i = 0;
		while (i < HASH_SIZE)
			(hashtable.table)[i++].fd = -1;
		hashtable.initialized = 1;
	}
	if (fd < 0)
		return (NULL);
	ft_putstr_fd(s, STDERR_FILENO);
	res.str = NULL;
	res.len = 0;
	res.cap = 0;
	node = ft_search_gnl(fd, hashtable.table);
	if (!node || !ft_get_and_put_c(node, &res))
		return (NULL);
	return (adjust_string(&res));
}
