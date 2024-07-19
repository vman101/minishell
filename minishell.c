/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/07/19 16:38:10 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int32_t	g_signal_flag;

void	setup(char **path_variable)
{
	g_signal_flag = 0;
	*path_variable = getenv("PATH");
	if (!path_variable)
		return (p_stderr(2, \
				"No Valid Path Variable was Found in Environment.\nExiting\n", \
				NULL), exit (1));
	terminal_raw_mode_enable();
	setup_signal_handlers();
	ft_printf(SCREEN_CLEAR);
	ft_printf(CURSOR_MOVE_HOME);
}

typedef struct s_tree_node
{
	t_type	type;
	char	*value;
	char	*full_path;
	char	**argv;
	bool	is_done;
	int32_t	fd_in;
	int32_t	fd_out;
	char	**env_ptr;
}				t_tree_node;

t_type is_file_or_directory(char *path)
{
	struct stat	sa;

	if (stat(path, &sa) == 0)
	{
		if (S_ISREG(sa.st_mode))
			return (EXISTING_FILE);
		else if (S_ISDIR(sa.st_mode))
			return (EXISTING_DIRECTORY);
	}
	return (UNDEFINED);
}

t_tree_node tree_node_create_from_string(t_token *token, const char *path_variable)
{
	t_tree_node	node;
	char		*value_ptr;

	node = (t_tree_node){0};
	node.value = token->value;
	if (token->symbol == TOKEN_STRING_LITERAL)
		return (node);
	node.argv = ft_split(node.value, ' ');
	if (!node.argv)
		lst_memory(NULL, NULL, CLEAN);
	node.full_path = find_absolute_path(path_variable, node.argv[0]);
	if (node.full_path)
		node.type = COMMAND;
	else
	{
		node.type = UNDEFINED;
		node.is_done = true;
	}
	return (node);
}

t_tree_node tree_node_create_from_redirection(t_token *token, t_token *next_token)
{
	t_tree_node	node;
	t_symbol	symbol;

	node = (t_tree_node){0};
	symbol = token->symbol;
	if (symbol == TOKEN_LESS_THAN)
		node.type = REDIRECTION_INPUT;
	if (symbol == TOKEN_MORE_THAN)
		node.type = REDIRECTION_OUTPUT;
	else if (symbol == TOKEN_DOUBLE_LESS_THAN)
		node.type = HERE_DOC;
	else if (symbol == TOKEN_DOUBLE_MORE_THAN)
		node.type = REDIRECTION_OUTPUT_APPEND;
	node.value = token->value;
	return (node);
}

t_tree_node tree_node_create_from_conditional(t_token *token)
{
	t_tree_node	node_new;

	node_new = (t_tree_node){0};
	return (node_new);
}

void	ast_print(t_tree_node *node)
{
	uint32_t	i;

	i = 0;
	while (node[i].type != FINAL)
	{
		ft_putstr_fd("AST\n", 1);
		ft_printf("%d\n%s\n%d\n", node[i].type, node[i].value, node[i].fd_in);
		ft_putchar_fd('\n', 1);
		i++;
	}
}

void	ast_interpret(t_tree_node *node, char **environment);

void	ast_create(t_token **tokens, const char *path_variable, char **environment)
{
	t_tree_node	*tree_node;
	uint32_t	i;

	i = 0;
	while (tokens[i])
		i++;
	tree_node = ft_calloc(i + 1, sizeof(*tree_node));
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->symbol < 2)
			tree_node[i] = tree_node_create_from_string(tokens[i], path_variable);
		else if (tokens[i]->symbol < 7)
			tree_node[i] = tree_node_create_from_redirection(tokens[i], tokens[i + 1]);
		else if (tokens[i]->symbol < 13)
	tree_node[i] = tree_node_create_from_conditional(tokens[i]);
		i++;
	}
	tree_node[i] = (t_tree_node){.type = FINAL, .is_done = true};
	ast_interpret(tree_node, environment);
	ft_free((void **)&tree_node);
}

uint32_t	find_logical_operator_or_pipe(t_tree_node *node)
{
	uint32_t	i;

	i = 0;
	while (node[i].type != FINAL)
	{
		if (node[i].type == PIPE || node[i].type == LOGICAL_AND || node[i].type == LOGICAL_OR)
			return (i);
		i++;
	}
	return (-1);
}

bool	is_all_done(t_tree_node *node)
{
	uint32_t	i;
	bool		are_undefined;

	i = 0;
	are_undefined = false;
	while (node[i].type != FINAL)
	{
		if (node[i].is_done == false)
			return (false);
		i++;
	}
	return (true);
}

bool	is_ready_for_command(t_tree_node *node)
{
	uint32_t	i;

	i = 0;
	while (node[i].type != FINAL)
	{
		if (node[i].type == COMMAND)
			;
		else if (node[i].is_done == false)
			return (false);
		i++;
	}
	return (true);
}

void	ast_interpret(t_tree_node *node, char **environment)
{
	uint32_t	command_start;
	uint32_t	command_end;
	uint32_t	i;
	int32_t		orig_stdout = dup(STDOUT_FILENO);
	int32_t		orig_stdin = dup(STDOUT_FILENO);

	i = 0;
	command_start = 0;
	command_end = find_logical_operator_or_pipe(node);
	while (!is_all_done(node))
	{
		if (node[i].type == FINAL)
			i = 0;
		if (node[i].is_done == true)
		{
			i++;
			continue ;
		}
		if (node[i].type == COMMAND)
		{
			if (is_ready_for_command(node) == true)
			{
				command_execute(node[i].full_path, (const char **)node[i].argv, (const char **)environment);
				node[i].is_done = true;
			}
		}
		else if (node[i].type == REDIRECTION_INPUT)
		{
			if (!node[i].value)
			{
				p_stderr(2, "minishell: syntax error near unexpected token `newline'\n", NULL);
				return ;
			}
			if (node[i].is_done == false && node[i].type != FINAL)
			{
				ft_open(&node[i].fd_in, node[i].value, O_RDONLY, 0644);
				if (!node[i].fd_in)
					return ;	
				ft_dup2(node[i].fd_in, STDIN_FILENO, "REDIRECTION_INPUT");
				node[i].is_done = true;
			}
		}
		else if (node[i].type == REDIRECTION_OUTPUT)
		{
			if (!node[i].value)
			{
				p_stderr(2, "minishell: syntax error near unexpected token `newline'\n", NULL);
				return ;
			}
			if (node[i].is_done == false && node[i].type != FINAL)
			{
				ft_open(&node[i].fd_out, node[i].value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (!node[i].fd_out)
					return ;	
				ft_dup2(node[i].fd_out, STDOUT_FILENO, "REDIRECTION_OUTPUT");
				node[i].is_done = true;
			}
		}
		else if (node[i].type == REDIRECTION_OUTPUT_APPEND)
		{
			if (node[i].type == EXISTING_FILE && node[i].is_done == false && node[i].type != FINAL)
				ft_open(&node[i].fd_out, node[i].value, O_WRONLY | O_APPEND, 0644);
			else if (node[i].type == COMMAND && node[i].argv[1] && node[i].is_done == false && node[i + 1].type != FINAL)
				ft_open(&node[i].fd_out, node[i].argv[1], O_WRONLY | O_APPEND, 0644);
			else if (node[i].type == UNDEFINED && node[i].is_done == false && node[i].type != FINAL)
				ft_open(&node[i].fd_out, node[i].value, O_WRONLY | O_CREAT, 0644);
			else
			{
				p_stderr(2, "-bash: syntax error near unexpected token `newline'\n", NULL);
				return ;
			}
			ft_dup2(node[i].fd_out, STDOUT_FILENO, "REDIRECTION_OUTPUT");
			node[i].is_done = true;
		}
		i++;
	}
	dup2(orig_stdout, STDOUT_FILENO);
	dup2(orig_stdin, STDIN_FILENO);
}

void	command_handler(char *command, char **environment, char *path_variable)
{
	t_token		**tokens;

	tokens = tokenizer(command, (const char **)environment);
	ast_create(tokens, path_variable, environment);
	/*while (tokens[i])*/
	/*{*/
	/*	ft_putstr_fd(tokens[i]->value, 1);*/
	/*	ft_putchar_fd('$', 1);*/
	/*	i++;*/
	/*}*/
	lst_memory(tokens, NULL, FREE);
}

int	main(int argc, char **argv, const char **env)
{
	t_prompt	*prompt;
	char		*path_variable;
	char		*command_input;
	char		**environment;

	(void)argv;
	(void)argc;
	setup(&path_variable);
	environment = environment_create(env);
	prompt = prompt_create((const char **)environment);
	while (g_signal_flag != 2)
	{
		command_input = prompt_get(prompt);
		if (g_signal_flag == 1 || !command_input)
		{
			if (g_signal_flag == 1)
			{
				ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1);
				ft_putchar_fd('\n', 1);
			}
			g_signal_flag = 0;
			continue ;
		}
		if (command_input && *command_input != '\n')
		{
			command_handler(command_input, environment, path_variable);
			prompt->history_entries[prompt->history_count++] = prompt->command;
		}
		prompt->history_position_current = prompt->history_count;
		ft_free((void **)&command_input);
	}
	terminal_raw_mode_disable();
}
