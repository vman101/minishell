/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:03:41 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/17 18:35:41 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '&');
}

static t_symbol	which_operator(char operator, bool is_double)
{
	if (operator == TOKEN_LESS_THAN)
	{
		if (is_double)
			return (TOKEN_DOUBLE_LESS_THAN);
		return (TOKEN_LESS_THAN);
	}
	else if (operator == TOKEN_MORE_THAN)
	{
		if (is_double)
			return (TOKEN_DOUBLE_MORE_THAN);
		return (TOKEN_MORE_THAN);
	}
	else if (operator == TOKEN_AMPERSAND)
	{
		if (is_double)
			return (TOKEN_DOUBLE_AMPERSAND);
		return (TOKEN_AMPERSAND);
	}
	else if (operator == TOKEN_PIPE)
	{
		if (is_double)
			return (TOKEN_DOUBLE_PIPE);
		return (TOKEN_PIPE);
	}
	return (TOKEN_DUMMY);
}

t_token	*find_operator(const char *command_input, uint32_t *i)
{
	t_token	*token;
	char	*return_value;
	bool	is_double;

	if (*command_input == *(command_input + 1))
	{
		return_value = ft_calloc(3, sizeof(*return_value));
		if (!return_value)
			lst_memory(NULL, NULL, CLEAN);
		ft_strlcpy(return_value, command_input, 3);
		(*i)++;
		is_double = true;
	}
	else
	{
		return_value = ft_calloc(2, sizeof(*return_value));
		if (!return_value)
			lst_memory(NULL, NULL, CLEAN);
		ft_strlcpy(return_value, command_input, 2);
		is_double = false;
	}
	(*i)++;
	token = token_create(return_value, which_operator(*command_input, is_double));
	return (token);
}

