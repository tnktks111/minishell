# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 19:06:08 by ttanaka           #+#    #+#              #
#    Updated: 2025/06/22 20:04:25 by ttanaka          ###   ########.fr        #
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
		exec/exec.c \
		exec/exec_utils1.c \
		exec/exec_utils2.c \
		exec/exec_builtin.c \
		exec/exec_solo_command.c \
		exec/exec_redirection.c \
		expander.c \
		file_expander.c \
		free.c \
		here_doc/here_doc.c \
		here_doc/here_doc_expander.c \
		here_doc/here_doc_quotes.c \
		here_doc/here_doc_utils.c \
		lexer.c \
		main.c \
		pattern_matching.c \
		signal.c \
		tempfile.c \
		parser.c \
		syntax_error.c

OBJS = $(patsubst %.c,$(OBJS_DIR)/%.o,$(SRCS))

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
		$(MKDIR) $(dir $@)
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