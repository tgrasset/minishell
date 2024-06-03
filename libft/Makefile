# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/09 10:48:17 by tgrasset          #+#    #+#              #
#    Updated: 2023/03/01 12:47:08 by tgrasset         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	./ft_isalpha.c \
			./ft_isalnum.c \
			./ft_isdigit.c \
			./ft_strlen.c	\
			./ft_strchr.c \
			./ft_strncmp.c \
			./ft_strnstr.c \
			./ft_atoi.c \
			./ft_strdup.c \
			./ft_strjoin.c \
			./ft_split.c \
			./ft_itoa.c \
			./ft_putchar_fd.c \
			./ft_putstr_fd.c \
			./ft_putendl_fd.c \
			./ft_putnbr_fd.c \

		
OBJS	=	$(SRCS:%.c=%.o)

NAME	=	libft.a

CC		=	cc

CFLAGS	=	-Wall -Wextra -Werror

all:		$(NAME)

%.o:		%.c
	$(CC) $(CFLAGS) -c $< -o $(<:%.c=%.o)

$(NAME):	$(OBJS)
	ar rc $(NAME) $(OBJS)
	ranlib $(NAME)

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean:		clean
	rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
