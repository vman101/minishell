/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/09 12:15:16 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	**custom_realloc(void ***args, int old_capacity, int new_capacity)
{
	void	**new_args;

	new_args = ft_calloc(new_capacity + 1, sizeof(void *));
	if (!new_args)
	{
		perror("Malloc failed while reallocing memory");
		lst_memory(NULL, NULL, CLEAN);
	}
	ft_memcpy(new_args, *args, old_capacity * sizeof(void *));
	free(*args);
	return (new_args);
}

void	fill_args(char ***args, int count, char *token_value, int *capacity)
{
	if (count >= *capacity)
	{
		*args = (char **)custom_realloc((void ***)args,
				*capacity, *capacity * 2);
		if (!args)
		{
			perror("calloc in parse tokens");
			lst_memory(NULL, NULL, CLEAN);
		}
		*capacity *= 2;
	}
	(*args)[count] = token_value;
}

/**/
/*void	parse_logical_operator(t_ast **head, int *i, t_token *tokens)*/
/*{*/
/*	t_ast	*current_node;*/
/**/
/*	current_node = NULL;*/
/*	current_node = create_logical_node(tokens[(*i)++].token_type);*/
/*	append_node(head, current_node);*/
/*}*/
/**/
/*void	parse_redirection(t_ast **head, int *i, t_token *tokens)*/
/*{*/
/*	t_ast	*current_node;*/
/*	char	*token_value;*/
/**/
/*	current_node = NULL;*/
/*	token_value = NULL;*/
/*	if (tokens[*i + 1].token_type != TOKEN_EOL)*/
/*	{*/
/*		token_value = tokens[*i + 1].token_value;*/
/*		current_node = create_redireciton_node(tokens[(*i)++].token_type, token_value);*/
/*	}*/
/*	(*i)++;*/
/*	append_node(head, current_node);*/
/*}*/
/**/
/*void	parse_pipe(t_ast **head, int *i, t_token *tokens)*/
/*{*/
/*	t_ast	*current_node;*/
/**/
/*	current_node = NULL;*/
/*	current_node = create_pipe_node(tokens[(*i)++].token_type);*/
/*	append_node(head, current_node);*/
/*}*/