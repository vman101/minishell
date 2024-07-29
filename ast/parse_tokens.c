/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:46:26 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 12:27:02 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_tokens(t_token *tokens)
{
	t_ast	*head;
	int		i;
	int		in_lst;

	if (!tokens)
		return (NULL);
	i = 0;
	in_lst = 0;
	head = NULL;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		if (tokens[i].token_type == TOKEN_WORD || tokens[i].token_type == TOKEN_ENV)
		{
			if (*tokens[i].token_value == 0)
			{
				i++;
				continue ;
			}
			parse_word(&head, &i, tokens);
		}
		else if (tokens[i].token_type == TOKEN_AND
			|| tokens[i].token_type == TOKEN_OR)
		{
			parse_logical_operator(&head, &i, tokens);
		}
		else if (is_redirection(tokens[i].token_type))
		{
			parse_redirection(&head, &i, tokens);
		}
		else if (tokens[i].token_type == TOKEN_PIPE)
		{
			parse_pipe(&head, &i, tokens);
		}
		if (head != NULL && !in_lst)
		{
			lst_memory((void *)head, clear_ast, ADD);
			in_lst = 1;
		}
		i++;
	}
	return (head);
}
