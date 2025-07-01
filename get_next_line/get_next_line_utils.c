/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 22:29:12 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 21:41:49 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putgnl_fd(char *str, int fd)
{
	size_t	len;

	len = ft_strlen(str);
	write(fd, str, len - 1);
}

int	ft_gnl_strcmp(char *gnlstr, char *s2)
{
	size_t	gnl_len;
	size_t	s_len;

	gnl_len = ft_strlen(gnlstr);
	s_len = ft_strlen(s2);
	if (ft_strncmp(gnlstr, s2, s_len) == 0 && gnl_len > s_len
		&& gnlstr[s_len] == '\n')
		return (0);
	return (1);
}

t_buf_node	*create_buf_node(int fd)
{
	t_buf_node	*newnode;
	t_buf		*buf;

	newnode = malloc(sizeof(t_buf_node));
	if (!newnode)
		return (NULL);
	newnode->fd = fd;
	newnode->next = NULL;
	buf = &(newnode->buffer);
	ft_bzero(buf->buf, BUFFER_SIZE);
	buf->bufp = buf->buf;
	buf->bufsiz = 0;
	return (newnode);
}

void	init_buf_node(t_buf_node *node, int fd)
{
	node->fd = fd;
	node->next = NULL;
	ft_bzero(node->buffer.buf, BUFFER_SIZE);
	node->buffer.bufp = node->buffer.buf;
	node->buffer.bufsiz = 0;
}

char	*adjust_string(t_string *res)
{
	char	*final_res;

	if (res->len == 0)
	{
		if (res->str)
			free(res->str);
		return (NULL);
	}
	final_res = (char *)malloc(sizeof(char) * (res->len) + 1);
	if (!final_res)
	{
		if (res->str)
			free(res->str);
		return (NULL);
	}
	ft_memmove(final_res, res->str, res->len);
	final_res[res->len] = '\0';
	if (res->str)
		free(res->str);
	return (final_res);
}
