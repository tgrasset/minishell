# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/01 14:58:06 by tgrasset          #+#    #+#              #
#    Updated: 2023/03/02 16:51:43 by tgrasset         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
########### VARIABLES

SRCS	=	./main.c \
			./lexer.c \
			./lexer_token_count.c \
			./lexer_token_extract.c \
			./parser.c \
			./parser_2.c \
			./var_expand.c \
			./var_expand_utils.c \
			./parser_utils.c \
			./parser_syntax_check.c \
			./free.c \
			./execution.c \
			./execution_cases.c \
			./execution_utils.c \
			./redirection.c \
			./redirection_utils.c \
			./env_setup.c \
			./built_in/built_in.c \
			./built_in/my_echo.c \
			./built_in/my_cd.c \
			./built_in/my_cd_utils.c \
			./built_in/my_pwd.c \
			./built_in/my_exit.c \
			./built_in/my_env.c \
			./built_in/my_export.c \
			./built_in/my_export_utils.c \
			./built_in/my_unset.c \
			
			
OBJS	=	$(SRCS:%.c=%.o)

NAME	=	minishell

CC		=	cc

CFLAGS	=	-Wall -Wextra -Werror -g

###############################################################################
########### RULES

all:		$(NAME)

$(NAME):	$(OBJS)
	make -C ./libft
	$(CC) $(CFLAGS) -lreadline $(OBJS) ./libft/libft.a -o $(NAME)

clean:
	make fclean -C ./libft
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
