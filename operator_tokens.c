/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:03:41 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/19 16:24:04 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&');
}

static t_symbol	which_operator(char operator, bool is_double)
{
	if (operator == '<')
	{
		if (is_double)
			return (TOKEN_DOUBLE_LESS_THAN);
		return (TOKEN_LESS_THAN);
	}
	else if (operator == '>')
	{
		if (is_double)
			return (TOKEN_DOUBLE_MORE_THAN);
		return (TOKEN_MORE_THAN);
	}
	else if (operator == '&')
	{
		if (is_double)
			return (TOKEN_DOUBLE_AMPERSAND);
		return (TOKEN_AMPERSAND);
	}
	else if (operator == '|')
	{
		if (is_double)
			return (TOKEN_DOUBLE_PIPE);
		return (TOKEN_PIPE);
	}
	return (TOKEN_DUMMY);
}

t_token	*find_operator(const char *command_input, uint32_t *i)
{
	t_token		*token;
	uint32_t	j;
	uint32_t	k;
	char		*return_value;
	bool		is_double;

	k = 0;
	return_value = NULL;
	if (*command_input == *(command_input + 1))
	{
		j = 2;
		is_double = true;
	}
	else if (*command_input + 1)
	{
		j = 1;
		is_double = false;
	}
	while (command_input[j] && command_input[j] == ' ')
		j++;
	while (command_input[j + k] && command_input[j + k] != ' ')
		k++;
	return_value = ft_substr(command_input, j, k);
	if (!return_value)
		lst_memory(NULL, NULL, CLEAN);
	*i += j + k;
	token = token_create(return_value, which_operator(*command_input, is_double));
	return (token);
}

