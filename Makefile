# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 19:06:08 by ttanaka           #+#    #+#              #
#    Updated: 2025/06/22 18:00:37 by ttanaka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
MKDIR = mkdir -p

OBJS_DIR = objs
LIBFT_DIR = libft

SRCS = 	builtin.c \
		builtin_cd.c \
		builtin_guruge.c \
		envp.c \
		error.c \
		exec.c \
		expander.c \
		file_expander.c \
		free.c \
		here_doc.c \
		lexer.c \
		main.c \
		pattern_matching.c \
		signal.c \
		tempfile.c \
		parser.c

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

LIBFT_A = $(LIBFT_DIR)/libft.a
INCS = -I. -I$(LIBFT_DIR)/inc

READLINE_LIB = -lreadline

LIBS = -L$(LIBFT_DIR) -lft $(READLINE_LIB)
CFLAGS += $(INCS)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(LIBFT_A):
		$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: %.c
		$(MKDIR) $(OBJS_DIR)
		$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
		$(RM) -r $(OBJS_DIR)
		$(MAKE) -C $(LIBFT_DIR) clean

fclean:
		$(RM) -r $(OBJS_DIR)
		$(RM) $(NAME)
		$(MAKE) -C $(LIBFT_DIR) fclean

re:		fclean all

.PHONY: all clean fclean re