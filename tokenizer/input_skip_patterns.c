/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_skip_patterns.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:27:01 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/21 11:13:32 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_delimiter_variable(char c)
{
	return (ft_isspace(c) || c == '\'' || c == '\"' || c == '$' || c == 0);
}

char	skip_to_delimiter(char *input_new)
{
	char	*temp_move;
	char	character_store;

	temp_move = input_new;
	while (ft_isalnum(*temp_move) || *temp_move == '?' || *temp_move == '_')
	{
		temp_move++;
	}
	character_store = *temp_move;
	*temp_move = 0;
	return (character_store);
}

bool	skip_single_quotes(char *input, uint32_t *i, bool *in_double_quotes)
{
	char	*temp_move;

	temp_move = input;
	if (input[*i] == '\"' && !*in_double_quotes)
		*in_double_quotes = true;
	else if (input[*i] == '\"' && *in_double_quotes)
		*in_double_quotes = false;
	if (input[*i] == '\'' && !*in_double_quotes)
	{
		temp_move = ft_strchr(&input[*i + 1], '\'');
		if (!temp_move)
			return (\
			ft_putendl_fd("minishell: missing closing single quote", 2), true);
		else
			*i = temp_move - input;
	}
	return (false);
}

bool	evaluate_double_quotes(char *input, uint32_t *i)
{
	uint32_t	input_length;

	input_length = ft_strlen(&input[*i]);
	ft_memmove(&input[*i], &input[*i + 1], input_length);
	while (input[*i])
	{
		if (input[*i] == '\"')
		{
			ft_memmove(&input[*i], &input[*i + 1], \
						ft_strlen(&input[*i]));
			(*i)--;
			return (false);
		}
		(*i)++;
	}
	return (ft_putendl_fd("minishell: missing closing double quote", 2), true);
}

bool	evaluate_single_quotes(char *input, uint32_t *i)
{
	uint32_t	input_length;

	input_length = ft_strlen(&input[*i]);
	ft_memmove(&input[*i], &input[*i + 1], input_length);
	while (input[*i])
	{
		if (input[*i] == '\'')
		{
			ft_memmove(&input[*i], &input[*i + 1], \
						ft_strlen(&input[*i]));
			(*i)--;
			return (false);
		}
		(*i)++;
	}
	return (ft_putendl_fd("minishell: missing closing single quote", 2), true);
}
