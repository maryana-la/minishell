# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjacquel <jjacquel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/11 10:35:24 by jjacquel          #+#    #+#              #
#    Updated: 2021/07/06 14:15:49 by jjacquel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= libft.a
CFLAGS  = -Wall -Wextra -Werror -g
RM		= rm -f
SRCS	=	ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c ft_split.c \
					ft_strlcat.c ft_strncmp.c ft_substr.c ft_atoi.c ft_isalpha.c \
					ft_itoa.c ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
					ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c ft_memccpy.c \
					ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
					ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  ft_memset.c  \
					ft_putstr_fd.c  ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_strcpy.c \
					get_next_line.c ft_strcmp.c
OBJ	= $(SRCS:.c=.o)

BONUS	=  ft_lstmap.c ft_lstiter.c	ft_lstclear.c   ft_lstdelone.c  ft_lstadd_back.c  ft_lstlast.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c

BONUS_OBJS	= $(BONUS:.c=.o)

%.o : %.c libft.h
	@$(CC) $(CFLAGS) -c $< -o $@

all:	$(NAME)

$(NAME):$(OBJ)
		ar rc $(NAME) $(OBJ) libft.h

clean:
		@$(RM) $(OBJ) $(BONUS_OBJS)

fclean:	clean
		$(RM) $(NAME)

re:		fclean $(NAME)

bonus:	$(OBJ) $(BONUS_OBJS)
		ar rc $(NAME) $(OBJ) $(BONUS_OBJS) libft.h

.PHONY:	all clean fclean re bonus

