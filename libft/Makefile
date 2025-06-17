# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/17 14:04:56 by ttanaka           #+#    #+#              #
#    Updated: 2025/06/17 16:01:39 by ttanaka          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

INC_DIR = inc
OBJS_DIR = objs

VPATH = is:lst:mem:others:put:str

SRCS = \
	is/ft_isalnum.c \
	is/ft_isalpha.c \
	is/ft_isascii.c \
	is/ft_isdigit.c \
	is/ft_isprint.c \
	mem/ft_bzero.c \
	mem/ft_calloc.c \
	mem/ft_memchr.c \
	mem/ft_memcmp.c \
	mem/ft_memcpy.c \
	mem/ft_memmove.c \
	mem/ft_memset.c \
	others/ft_atoi.c \
	others/ft_itoa.c \
	others/ft_split.c \
	others/ft_tolower.c \
	others/ft_toupper.c \
	put/ft_putchar_fd.c \
	put/ft_putendl_fd.c \
	put/ft_putnbr_fd.c \
	put/ft_putstr_fd.c \
	str/ft_strchr.c \
	str/ft_strcmp.c \
	str/ft_strdup.c \
	str/ft_striteri.c \
	str/ft_strjoin.c \
	str/ft_strlcat.c \
	str/ft_strlcpy.c \
	str/ft_strlen.c \
	str/ft_strmapi.c \
	str/ft_strncmp.c \
	str/ft_strnstr.c \
	str/ft_strrchr.c \
	str/ft_strtrim.c \
	str/ft_substr.c

BONUS_SRCS = \
	lst/ft_lstadd_back_bonus.c \
	lst/ft_lstadd_front_bonus.c \
	lst/ft_lstclear_bonus.c \
	lst/ft_lstdelone_bonus.c \
	lst/ft_lstiter_bonus.c \
	lst/ft_lstlast_bonus.c \
	lst/ft_lstmap_bonus.c \
	lst/ft_lstnew_bonus.c \
	lst/ft_lstsize_bonus.c

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))
BONUS_OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(BONUS_SRCS:.c=.o)))

INC = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

bonus: $(OBJS) $(BONUS_OBJS)
	$(AR) $(NAME) $^

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus