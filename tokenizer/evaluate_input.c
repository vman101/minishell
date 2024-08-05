/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:24:06 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	evaluate_variable(char **input, uint32_t input_length, const char **environment)
{
	char		*input_new;
	uint32_t	i;
	char		*temp_move;
	char		character_store;
	char		*string_to_insert;
	uint32_t	string_to_insert_length;

	input_new = ft_calloc(VARIABLE_TOKEN_SIZE, sizeof(*input_new));
	if (!input_new)
	{
		perror("ft_calloc");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	ft_strlcpy(input_new, *input, input_length + 1);
	i = 0;
	character_store = 0;
	while ((input_new)[i])
	{
		if (input_new[i] == '\'')
		{
			if (!ft_strchr(&input_new[i + 1], '\''))
			{
				p_stderr(2, "%s: Invalid SINGLE Quotes\n", *input);
				lst_memory(NULL, NULL, CLEAN, 0);
			}
			else
			{
				while (input_new[i] != '\'')
					i++;
			}
		}
		if ((input_new)[i] == '$')
		{
			ft_memmove(&(input_new)[i], &(input_new)[i + 1], (input_length - i));
			temp_move = ft_strchr(&(input_new)[i], '\'');
			if (temp_move)
			{
				character_store = *temp_move;
			}
			else
			{
				temp_move = ft_strchr(&(input_new)[i], ' ');
				if (!temp_move)
					temp_move = ft_strchr(&(input_new)[i], '\"');
				if (!temp_move)
					temp_move = ft_strchr(&(input_new)[i], '$');
				if (!temp_move)
					temp_move = ft_strchr(&(input_new)[i], 0);
				if (temp_move && *temp_move)
					ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
			}
			*temp_move = 0;
			string_to_insert = environment_variable_value_get(&(input_new)[i], (const char **)environment);
			string_to_insert_length = ft_strlen(string_to_insert);
			input_length = ft_strlen(input_new);
			ft_memmove(&input_new[i + input_length + string_to_insert_length], &input_new[i + input_length], input_length - i);
			ft_memcpy(&input_new[i], string_to_insert, string_to_insert_length);
			*temp_move = character_store;
		}
		(i)++;
	}
	*input = input_new;
	return (ft_strlen(input_new));
}

void	evaluate_double_quotes(char **input, uint32_t *i, uint32_t input_length, const char **environment)
{
	char	*temp_move;
	char	*string_to_insert;
	uint32_t	string_to_insert_length;
	char		character_store;

	character_store = 0;
	ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
	while ((*input)[*i])
	{
		if ((*input)[*i] == '\"')
		{
			ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
			break ;
		}
		if ((*input)[*i] == '$')
		{
			ft_memmove(&(*input)[*i], &(*input)[*i + 1], (input_length - *i));
			temp_move = ft_strchr(&(*input)[*i], ' ');
			if (temp_move)
				*temp_move = 0;
			temp_move = ft_strchr(&(*input)[*i], '\"');
			if (temp_move)
				*temp_move = 0;
			temp_move = ft_strchr(&(*input)[*i], '$');
			if (temp_move)
			{
				character_store = '$';
				*temp_move = 0;
			}
			string_to_insert = environment_variable_value_get(&(*input)[*i], (const char **)environment);
			string_to_insert_length = ft_strlen(string_to_insert);
			ft_memmove(&(*input)[*i], temp_move, ft_strlen(*input));
			handle_multiple_character_to_input(input, string_to_insert, (uint32_t [2]){0, *i}, input_length);
		}
		(*i)++;
	}
}

void	evaluate_single_quotes(char **input, uint32_t *i, uint32_t input_length)
{
	char	*temp_move;

	ft_memmove(&(*input)[*i], &(*input)[*i + 1], input_length - *i);
	temp_move = ft_strchr(&(*input)[*i], '\'');
	if (!temp_move)
	{
		UNIMPLEMENTED("Error single quote missing");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
}

char	**evaluate_input(char ***input, const char **environment)
{
	uint32_t	input_length;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (input[i])
	{
		input_length = ft_strlen((*input)[i]);
		j = 0;
		if (ft_strchr((*input)[i], '$'))
		{
			input_length = evaluate_variable(input[i], input_length, environment);
		}
		while ((*input)[i][j])
		{
			if ((*input)[i][j] == '\"')
				evaluate_double_quotes(input[i], &j, input_length, environment);
			else if ((*input)[i][j] == '\'')
				evaluate_single_quotes(input[i], &j, input_length);
			j++;
		}
		i++;
	}
	return (*input);
}
