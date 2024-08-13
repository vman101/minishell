/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/13 15:02:28 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdint.h>

void	insert_string(char *input_new, char *string_to_insert, uint32_t *i, char character_store)
{
	char		*temp_move;
	uint32_t	string_to_insert_length;
	uint32_t	input_length;

	temp_move = ft_strchr(&input_new[*i], 0);
	if (!string_to_insert)
		return (ft_memmove(input_new, temp_move, ft_strlen(input_new)), *input_new = character_store, (void)0);
	if (character_store)
		*temp_move = character_store;
	string_to_insert_length = ft_strlen(string_to_insert);
	input_length = ft_strlen(input_new);
	ft_memmove(&input_new[*i + string_to_insert_length], \
			&input_new[temp_move - input_new], \
			input_length - (temp_move - input_new) + 1);
	ft_memcpy(&input_new[*i], string_to_insert, string_to_insert_length);
	*i += string_to_insert_length;
}

void	insert_exit_status(char *input_new, int32_t *exit_status, uint32_t *input_length, char character_store)
{
	char		*exit_status_string;
	uint32_t	i;

	exit_status_string = ft_itoa(*exit_status);
	i = 0;
	while (exit_status_string[i])
	{
		ft_memmove(&input_new[i + 1], &input_new[i], ft_strlen(input_new) - i);
		input_new[i] = exit_status_string[i];
		(*input_length)++;
		i++;
	}
	input_new[i] = character_store;
	ft_free(&exit_status_string);
}

bool	is_delimiter(char c)
{
	return (c == ' ' || c == '\'' || c == '\"' || c == '$');
}

char	skip_to_delimiter(char *input_new)
{
	char	*temp_move;
	char 	character_store;

	temp_move = input_new;
	while (temp_move[0] && !is_delimiter(temp_move[0]))
		temp_move++;
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
			return (ft_putendl_fd("minishell: missing closing single quote", 2), true);
		else
			*i = temp_move - input + 1;
	}
	return (false);
}

uint32_t	evaluate_variable(	char **input, \
								uint32_t *input_length, \
								const char **environment, \
								int32_t *exit_status)
{
	char		*input_new;
	uint32_t	i;
	char		character_store;
	bool		in_double_quotes;

	in_double_quotes = false;
	input_new = ft_calloc(*input_length * 2, sizeof(*input_new));
	lst_memory(input_new, free, ADD);
	ft_strlcpy(input_new, *input, *input_length + 1);
	i = 0;
	while (input_new[i])
	{
		if (skip_single_quotes(input_new, &i, &in_double_quotes))
			return (true);
		if (input_new[i] == '$')
		{
			ft_memmove(&input_new[i], &input_new[i + 1], *input_length - i);
			character_store = skip_to_delimiter(&input_new[i]);
			if (input_new[i] == '?' && is_delimiter(input_new[i + 1]))
				insert_exit_status(input_new, exit_status, input_length, character_store);
			else
				insert_string(input_new, environment_variable_value_get(&input_new[i], environment), &i, character_store);
			continue ;
		}
		i++;
	}
	*input = input_new;
	return (false);
}

bool	evaluate_double_quotes(char **input, uint32_t *i, uint32_t *input_length)
{
	ft_memmove(&(*input)[*i], &(*input)[*i + 1], *input_length - *i + 1);
	(*input_length)--;
	while ((*input)[*i])
	{
		if ((*input)[*i] == '\"')
		{
			ft_memmove(&(*input)[*i], &(*input)[*i + 1], *input_length - *i + 1);
			(*i)--;
			(*input_length)--;
			return (false);
		}
		(*i)++;
	}
	return (ft_putendl_fd("minishell: missing closing double quote", 2), true);
}

bool	evaluate_single_quotes(char **input, uint32_t *i, uint32_t *input_length)
{
	ft_memmove(&(*input)[*i], &(*input)[*i + 1], *input_length - *i);
	(*input_length)--;
	while ((*input)[*i] && (*input)[*i] != '\'')
		(*i)++;
	if (!(*input)[*i] || (*input)[*i] != '\'')
		return (ft_putendl_fd("minishell: missing closing single quote", 2), true);
	ft_memmove(&(*input)[*i], &(*input)[*i + 1], *input_length - *i);
	(*input_length)--;
	return (false);
}

void	evaluate_input(char ***input, const char **environment, int32_t *exit_status, bool error_caught)
{
	uint32_t	input_length;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	error_caught = false;
	while ((*input)[i] && !error_caught)
	{
		input_length = ft_strlen((*input)[i]);
		j = 0;
		if (ft_strchr((*input)[i], '$'))
			error_caught = evaluate_variable(&(*input)[i], &input_length, \
											environment, exit_status);
		while ((*input)[i][j] && !error_caught)
		{
			if ((*input)[i][j] == '\"')
				error_caught = evaluate_double_quotes(&(*input)[i], &j, &input_length);
			else if ((*input)[i][j] == '\'')
				error_caught = evaluate_single_quotes(&(*input)[i], &j, &input_length);
			j++;
		}
		i++;
	}
}