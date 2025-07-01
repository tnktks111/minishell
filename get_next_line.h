/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 22:29:08 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 20:43:47 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
// # include <fcntl.h>

# define MALLOC_ERR -2
# define RD_ERR -3

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# if BUFFER_SIZE < 1
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define HASH_SIZE 1024

# if BUFFER_SIZE >= 1000000
#  undef HASH_SIZE
#  define HASH_SIZE 1000
# endif

# if BUFFER_SIZE >= 10000000
#  undef HASH_SIZE
#  define HASH_SIZE 100
# endif

typedef struct s_string
{
	char				*str;
	size_t				len;
	size_t				cap;
}						t_string;

typedef struct s_buf
{
	char				buf[BUFFER_SIZE];
	char				*bufp;
	int					bufsiz;
}						t_buf;

typedef struct s_buf_node
{
	int					fd;
	t_buf				buffer;
	struct s_buf_node	*next;
}						t_buf_node;

typedef struct s_hashtable
{
	t_buf_node			table[HASH_SIZE];
	int					initialized;
}						t_hashtable;

void					ft_bzero(void *s, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t len);
t_buf_node				*create_buf_node(int fd);
void					init_buf_node(t_buf_node *node, int fd);
char					*adjust_string(t_string *res);

int						ft_getc(t_buf_node *node);
int						ft_putc(t_string *str, char c);
t_buf_node				*ft_search_gnl(int fd, t_buf_node *hashtable);
int						ft_get_and_put_c(t_buf_node *node, t_string *res);
char					*get_next_line(char *s, int fd);

#endif