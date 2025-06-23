# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 19:06:08 by ttanaka           #+#    #+#              #
#    Updated: 2025/06/23 10:53:46 by ttanaka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
MKDIR = mkdir -p

OBJS_DIR = objs
LIBFT_DIR = libft

SRCS = 	builtin/builtin_cd.c \
		builtin/builtin_echo.c \
		builtin/builtin_env.c \
		builtin/builtin_exit.c \
		builtin/builtin_export.c \
		builtin/builtin_pwd.c \
		builtin/builtin_unset.c \
		envp/envp.c \
		envp/envp_utils1.c \
		envp/envp_utils2.c \
		envp/envp_utils3.c \
		envp/handle_shlvl.c \
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
		here_doc/here_doc_tmpfile.c \
		here_doc/here_doc_utils.c \
		lexer.c \
		main.c \
		pattern_matching.c \
		signal.c \
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