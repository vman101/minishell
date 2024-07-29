/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 15:45:21 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	**custom_realloc(void **args, int old_capacity, int new_capacity)
{
	int		i;
	void	**new_args;

	new_args = ft_calloc(new_capacity + 1, sizeof(void *));
	if (!new_args)
	{
		perror("Malloc failed while reallocing memory");
		lst_memory(NULL, NULL, CLEAN);
	}
	i = 0;
	while (i < old_capacity)
	{
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
		{
			perror("Strdup failed while reallocing memory");
			lst_memory(NULL, NULL, CLEAN);
		}
		i++;
	}
	return (new_args);
}

void	fill_args(char **args, int count, char *token_value, int *capacity)
{
	args[count] = ft_strdup(token_value);
	if (!args[count])
	{
		perror("strdup fopr token value in parsing");
		lst_memory(NULL, NULL, CLEAN);
	}
	if (count >= *capacity)
	{
		args = (char **)custom_realloc((void **)args,
				*capacity, *capacity * 2);
		if (!args)
		{
			perror("calloc in parse tokens");
			lst_memory(NULL, NULL, CLEAN);
		}
		*capacity *= 2;
	}
}

void	parse_word(t_ast **head, int *i, t_token *tokens)
{
	char	**args;
	int		capacity;
	int		count;
	t_ast	*current_node;

	count = 0;
	current_node = NULL;
	capacity = INITIAL_TOKEN_CAPACITY;
	args = ft_calloc(capacity + 1, sizeof(char *));
	if (!args)
	{
		perror("calloc in parse tokens");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(args, free_split, ADD);
	while (tokens[*i].token_type != TOKEN_EOL && (tokens[*i].token_type == TOKEN_WORD || tokens[*i].token_type == TOKEN_ENV))
	{
		fill_args(args, count, tokens[*i].token_value, &capacity);
		count++;
		(*i)++;
	}
	(*i)--;
	current_node = create_command_node(tokens[*i].token_type, args);
	append_node(head, current_node);
}

void	parse_logical_operator(t_ast **head, int *i, t_token *tokens)
{
	t_ast	*current_node;

	current_node = NULL;
	current_node = create_logical_node(tokens[*i].token_type);
	append_node(head, current_node);
}

void	parse_redirection(t_ast **head, int *i, t_token *tokens)
{
	int		len_tokens;
	t_ast	*current_node;

	current_node = NULL;
	len_tokens = get_tokens_len(tokens);
	if (*i + 1 < len_tokens && tokens[*i + 1].token_type == TOKEN_WORD)
	{
		current_node = create_redireciton_node(tokens[*i].token_type,
				tokens[*i + 1].token_value);
		(*i)++;
	}
	else 
	{
		current_node = create_redireciton_node(tokens[*i].token_type,
				NULL);
	}
	append_node(head, current_node);
}

void	parse_pipe(t_ast **head, int *i, t_token *tokens)
{
	t_ast	*current_node;

	current_node = NULL;
	current_node = create_pipe_node(tokens[*i].token_type);
	append_node(head, current_node);
}
