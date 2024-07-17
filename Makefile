# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anarama <anarama@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 12:24:47 by victor            #+#    #+#              #
#    Updated: 2024/07/17 17:23:01 by vvobis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell

TEST_NAME	:= test

CC			:= cc

CFLAGS		:= -Wall -Wextra -g
#-fsanitize=address,undefined,leak -static-libsan

LIBS	:= libft/libft.a -lreadline

SRC 	:=	arrowkeys.c					\
			builtins.c					\
			commands.c					\
			environment_variables.c		\
			escape_sequences.c			\
			handle_signals.c			\
			input.c						\
			list_memory.c				\
			list.c						\
			minishell.c					\
			operator_tokens.c			\
			path_utils.c				\
			prompt_input.c				\
			prompt_string_management.c	\
			prompt_utils.c				\
			quotes.c					\
			tab_completion.c			\
			termios.c					\
			tokenizer.c					\
			utils.c						\
			redirections.c				\
			ft_echo.c					\
			ft_pwd.c					\
			dollar_sign.c				\
			ft_env.c					\
			utils2.c					\

TEST_SRC :=	arrowkeys.c					\
			builtins.c					\
			commands.c					\
			environment_variables.c		\
			escape_sequences.c			\
			handle_signals.c			\
			input.c						\
			list_memory.c				\
			list.c						\
			path_utils.c				\
			prompt_input.c				\
            prompt_string_management.c	\
            prompt_utils.c				\
            tab_completion.c			\
            termios.c					\
            utils.c						\
            redirections.c				\
            ft_echo.c					\
            ft_pwd.c					\
            dollar_sign.c				\
            ft_env.c					\
            utils2.c					\
			tokenizer.c

OBJ := $(SRC:%.c=%.o)

TEST_OBJ := $(TEST_SRC:%.c=%.o)

BONOBJ := $(BONSRC:%.c=%.o)

all: $(NAME)

bonus: $(BONNAME)

$(NAME): $(OBJ) $(LIBS) minishell.h
	make -C libft all
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

$(BONNAME): $(BONOBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) -o $(BONNAME) $(BONOBJ) $(LIBS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^

$(LIBS):
	make bonus -C libft

test: $(TEST_OBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) $(TEST_OBJ) $(LIBS) -o $(TEST_NAME)

clean:
	make clean -C libft
	rm -f $(OBJ) $(BONOBJ)

fclean: clean
	make fclean -C libft
	rm -f $(NAME) $(BONNAME)

re: fclean all
