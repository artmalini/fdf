# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/24 13:52:27 by amakhiny          #+#    #+#              #
#    Updated: 2017/10/24 13:53:44 by amakhiny         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
GC = $(CFLAGS) -O2
CFLAGS = -Wall -Wextra -Werror
INC =  -I libft.h
SOURCE = ft_strlen.c ft_strstr.c ft_memdel.c ft_bzero.c ft_strdup.c ft_putnbr.c ft_putchar.c ft_strjoin.c ft_strnew.c ft_memalloc.c \
		ft_strcpy.c ft_strdel.c ft_strsub.c ft_minint.c ft_strchr.c ft_int_length.c ft_atoi.c ft_itoa.c get_next_line.c \
		ft_putstr.c ft_strsplit.c ft_count_words.c ft_getword_len.c

OBJ = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME):
	@gcc $(GC) -c $(SOURCE) $(INC)
	@ar rc $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
