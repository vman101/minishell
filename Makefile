# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anarama <anarama@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 12:24:47 by victor            #+#    #+#              #
#    Updated: 2024/07/29 12:18:52 by anarama          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COMPILER AND FLAGS
CC			:= cc
CFLAGS		:= -Wall -Wextra -g

# DIRECTORIES
SRCDIR		:= src
ASTDIR		:= ast
OBJDIR		:= obj
TOKENDIR	:= tokenizer

SRC			:=	src/arrowkeys.c src/builtins.c src/commands.c \
				src/environment_variables.c src/escape_sequences.c \
				src/ft_echo.c src/ft_env.c src/ft_pwd.c src/handle_signals.c \
				src/input.c src/list_memory.c src/list.c src/minishell.c \
				src/path_utils.c src/prompt_input.c src/prompt_string_management.c \
				src/prompt_utils.c src/tab_completion.c src/termios.c \
				src/utils.c src/utils2.c

AST_SRC		:=	ast/ast_create_node.c ast/ast_print.c ast/ast_utils.c \
				ast/handle_command.c ast/handle_fds.c \
				ast/parse_tokens.c ast/parser.c ast/handle_redirs.c \
				ast/handle_pipes.c ast/handle_logical_operator.c \
				ast/syntax_checks.c

TOKEN_SRC	:=	tokenizer/check_special_symbol.c \
				tokenizer/create_token_double_special_symbol.c \
				tokenizer/create_token_env_var.c \
				tokenizer/create_token_quotes.c \
				tokenizer/create_token_single_special_symbol.c \
				tokenizer/create_token_word.c \
				tokenizer/create_token.c \
				tokenizer/string_utils.c \
				tokenizer/quotes.c \
				tokenizer/tokenizer.c

TEST_SRC	:=	src/arrowkeys.c src/builtins.c src/commands.c src/dollar_sign.c \
				src/environment_variables.c src/escape_sequences.c \
				src/ft_echo.c src/ft_env.c src/ft_pwd.c \
				src/input.c src/list_memory.c src/list.c \
				src/path_utils.c src/prompt_string_management.c \
				src/redirections.c src/tab_completion.c src/termios.c \
				src/test.c src/utils.c src/utils2.c

# OBJECT FILES
OBJ			:= $(SRC:%.c=$(OBJDIR)/%.o)
AST_OBJ		:= $(AST_SRC:ast/%.c=$(OBJDIR)/ast/%.o)
TEST_OBJ	:= $(TEST_SRC:%.c=$(OBJDIR)/%.o)
TOKEN_OBJ	:= $(TOKEN_SRC:tokenizer/%.c=$(OBJDIR)/tokenizer/%.o)

NAME		:= minishell
LIBS		:= libft/libft.a
TEST_NAME	:= test

# Create object directory if none exists
$(shell mkdir -p $(OBJDIR) $(OBJDIR)/ast $(OBJDIR)/src $(OBJDIR)/tokenizer)

all: $(NAME)

$(NAME): $(OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) $(OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) -o $(NAME)

test: $(TEST_OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) $(TEST_OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) -o $(TEST_NAME)

clean:
	make clean -C libft
	rm -f $(OBJ) $(AST_OBJ) $(TEST_OBJ) $(TOKEN_OBJ)

fclean: clean
	make fclean -C libft
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/ast/%.o: ast/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/tokenizer/%.o: tokenizer/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBS):
	make bonus -C libft
