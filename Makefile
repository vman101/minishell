# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anarama <anarama@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 12:24:47 by victor            #+#    #+#              #
#    Updated: 2024/08/26 18:59:31 by vvobis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COMPILER AND FLAGS
CC			:= cc
CFLAGS		:= -Wall -Wextra -g3

# DIRECTORIES
SRCDIR		:= src
ASTDIR		:= ast
BULDINDIR	:= builtin
OBJDIR		:= obj
TOKENDIR	:= tokenizer

SRC			:=	src/commands.c src/environment_variables.c \
				src/handle_signals.c src/input.c src/list_memory.c \
				src/list.c src/minishell.c \
				src/path_utils.c src/termios.c \
				src/utils.c src/utils2.c src/utils3.c \
				src/environment_variables_manip.c \
				src/wildcards.c src/execution.c \
				src/non_interactive.c src/wildcards_hanlde_match.c \
				src/wildcards_helper.c

PROMPT_SRC	:= 	prompt/prompt_input.c prompt/prompt_string_management.c \
				prompt/prompt_utils.c prompt/tab_completion.c \
				prompt/escape_sequences.c prompt/arrowkeys.c \
				prompt/prompt_print.c prompt/tab_get_word.c \
				prompt/non_blocking_mode.c prompt/prompt_handle_chars.c \

AST_SRC		:=	ast/ast_utils.c \
				ast/handle_command.c ast/handle_fds.c \
				ast/parse_tokens.c ast/parser.c \
				ast/syntax_check.c ast/handle_redirs.c \
				ast/handle_heredoc.c ast/parse_tokens_helper.c \
				ast/syntax_check_helper.c ast/buildin.c

TOKEN_SRC	:=	tokenizer/check_special_symbol.c \
				tokenizer/create_token_double_special_symbol.c \
				tokenizer/create_token_single_special_symbol.c \
				tokenizer/create_token_word.c \
				tokenizer/create_token.c \
				tokenizer/tokenizer.c \
				tokenizer/subshell.c tokenizer/evaluate_input.c \
				tokenizer/input_skip_patterns.c \
				tokenizer/token_heredoc.c \
				tokenizer/tokenizer_heredoc_helper.c

BUILDIN_SRC :=	builtin/ft_echo.c builtin/ft_env.c \
				builtin/ft_pwd.c builtin/ft_unset.c \
				builtin/ft_export.c builtin/ft_exit.c \
				builtin/ft_cd.c

# OBJECT FILES
OBJ			:= $(SRC:%.c=$(OBJDIR)/%.o)
AST_OBJ		:= $(AST_SRC:ast/%.c=$(OBJDIR)/ast/%.o)
BUILDIN_OBJ	:= $(BUILDIN_SRC:builtin/%.c=$(OBJDIR)/builtin/%.o)
PROMPT_OBJ	:= $(PROMPT_SRC:prompt/%.c=$(OBJDIR)/prompt/%.o)
TOKEN_OBJ	:= $(TOKEN_SRC:tokenizer/%.c=$(OBJDIR)/tokenizer/%.o)

NAME		:= minishell
LIBS		:= libft/libft.a

# Create object directory if none exists
$(shell mkdir -p $(OBJDIR) $(OBJDIR)/ast $(OBJDIR)/src $(OBJDIR)/tokenizer $(OBJDIR)/builtin $(OBJDIR)/prompt)

all: $(NAME)

$(NAME): $(OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(BUILDIN_OBJ) $(PROMPT_OBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) $(OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(BUILDIN_OBJ) $(PROMPT_OBJ) $(LIBS) -o $(NAME)

test: $(TEST_OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) minishell.h
	$(CC) $(CFLAGS) $(TEST_OBJ) $(AST_OBJ) $(TOKEN_OBJ) $(LIBS) -o $(TEST_NAME)

clean:
	make clean -C libft
	rm -f $(OBJ) $(AST_OBJ) $(TEST_OBJ) $(TOKEN_OBJ) $(PROMPT_OBJ) $(BUILDIN_OBJ)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)
	make fclean -C libft

re: fclean all

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/builtin/%.o: builtin/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/prompt/%.o: prompt/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/ast/%.o: ast/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/tokenizer/%.o: tokenizer/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBS):
	make bonus -C libft
