#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhallste <rhallste@student.42.us.org>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/09/18 09:37:14 by rhallste          #+#    #+#              #
#    Updated: 2018/03/07 00:56:29 by rhallste         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		= 	ft_ssl
CC			=	gcc
EXTRA_FLAGS =	-Weverything -Wno-cast-qual
CFLAGS		=	-Wall -Werror -Wextra

BASE64		=	base64/base64

DES			=	$(addprefix des/,	\
				des					\
				permute				\
				boxes				)

FT_SSL		=	main 		\
				args		\
				error		\
				opt			\
				padding		\
				prep_args

FILES		=	$(FT_SSL) $(BASE64) $(DES)

SRCS		=	$(addprefix src/, $(addsuffix .c, $(FILES)))
OBJS		=	$(addprefix src/, $(addsuffix .o, $(FILES)))

LIBFT		=	 inc/libft/libftprintf.a

all: $(NAME)

$(OBJS): $(SRCS)

$(LIBFT):
	@make -C inc/libft

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	@rm -rf $(OBJS)
	@make -C inc/libft clean

fclean: clean
	@rm -rf $(NAME)
	@make -C inc/libft fclean

re: fclean all

.SILENT: $(OBJS) $(LIBFT)
