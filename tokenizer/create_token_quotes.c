/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:38:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 17:22:20 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(const char input)
{
	return (input == '\'' || input == '\"');
}

t_token	create_token_single_quote(const char **input)
{
	t_token	temp_token;
	char	*temp_move;

	temp_move = ft_strchr(*input + 1, '\'');
	if (!temp_move)
	{
		p_stderr(2, "Missing closing quote in: %s\n", *input);
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	temp_token = create_token(TOKEN_WORD, (*input) + 1);
	if (temp_move)
	{
		*temp_move = 0;
		*input = temp_move + 1;
	}
	else
		*input = ft_strchr(*input, 0);
	return (temp_token);
}

t_token	create_token_quotes(const char **input, const char **env)
{
	t_token	temp_token;

	temp_token = (t_token){0};
	if (**input == '\'')
	{
		temp_token = create_token_single_quote(input);
	}
	else if (**input == '\"')
	{
		temp_token.token_value = interpret_double_quotes(input, &temp_token.token_type);
	}
	return (temp_token);
}
