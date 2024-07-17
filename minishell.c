/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/07/17 22:39:11 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_type	node_type;
	char	*value;
	char	**argv;
	int32_t	fd_in;
	int32_t	fd_out;
	char	**env_ptr;
}				t_tree_node;

uint8_t	is_file_or_directory(char *path)
{
	struct stat	sa;
	mode_t t;

	if (stat(path, &sa) == 0)
	{
		if (S_ISREG(sa.st_mode) == 0)
			return (1);
		else if (S_ISDIR(sa.st_mode) == 0)
			return (2);
	}
	return (0);
}

t_tree_node tree_node_create_from_string(t_token *token, const char *path_variable)
{
	t_tree_node	node;
	char		*value_ptr;

	node = (t_tree_node){0};
	if (token->symbol == TOKEN_STRING_LITERAL)
	{
		node.value = token->value;
		token->value = NULL;
		return (node);
	}
	value_ptr = find_absolute_path(path_variable, token->value);
	return (node);
}

t_tree_node tree_node_create_from_redirection(t_token *token)
{
	t_tree_node	node_new;

	node_new = (t_tree_node){0};
	return (node_new);
}

t_tree_node tree_node_create_from_conditional(t_token *token)
{
	t_tree_node	node_new;

	node_new = (t_tree_node){0};
	return (node_new);
}

void	ast_create(t_token **tokens, const char *path_variable)
{
	t_tree_node	*tree_node;
	uint32_t	i;

	while (tokens[i])
	{
		ft_putstr_fd(tokens[i]->value, 1);
		i++;
	}
	tree_node = ft_calloc(i, sizeof(*tree_node));
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->symbol < 2)
			tree_node[i] = tree_node_create_from_string(tokens[i], path_variable);
		else if (tokens[i]->symbol < 7)
			tree_node[i] = tree_node_create_from_redirection(tokens[i]);
		else if (tokens[i]->symbol < 13)
			tree_node[i] = tree_node_create_from_conditional(tokens[i]);
		i++;
	}
}

void	command_handler(char *command, char **environment)
{
	t_token		**tokens;
	uint32_t	i;

	i = 0;
	tokens = tokenizer(command, (const char **)environment);
	while (tokens[i])
	{
		ft_putstr_fd(tokens[i]->value, 1);
		ft_putchar_fd('$', 1);
		i++;
	}
	lst_memory(tokens, NULL, FREE);
	ft_putchar_fd('\n', 1);
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
			command_handler(command_input, environment);
			prompt->history_entries[prompt->history_count++] = prompt->command;
		}
		prompt->history_position_current = prompt->history_count;
		ft_free((void **)&command_input);
	}
	terminal_raw_mode_disable();
}
