/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 19:09:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_string(	char **input_new, \
						char *string_to_insert, \
						char character_store, \
						uint32_t i)
{
	char		*temp_move;
	uint32_t	string_to_insert_length;
	uint32_t	input_length;

	if (string_to_insert)
	{
		string_to_insert_length = ft_strlen(string_to_insert);
		*input_new = prompt_buffer_size_manage(input_new, \
				ft_strlen(*input_new), ft_strlen(*input_new) \
				+ string_to_insert_length + 1, VARIABLE_TOKEN_SIZE);
	}
	input_length = ft_strlen(*input_new + i);
	temp_move = ft_strchr(*input_new + i, 0);
	*temp_move = character_store;
	if (!string_to_insert)
	{
		ft_bzero(*input_new + i, input_length);
		ft_memmove(*input_new + i, temp_move, ft_strlen(temp_move) + 1);
		return ;
	}
	ft_memmove(*input_new + i + string_to_insert_length, temp_move, \
			ft_strlen(temp_move) + 1);
	ft_memcpy(*input_new + i, string_to_insert, string_to_insert_length);
}

void	insert_exit_status(	char *input_new, \
							int32_t *exit_status, \
							uint32_t *input_length, \
							char character_store)
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

void	evaluate_helper(char **input_new, uint32_t *input_length, \
						int32_t *exit_status, uint32_t i)
{
	char	character_store;
	char	**environment;
	char	*variable;

	environment = env_static(NULL);
	ft_memmove(*input_new + i, (*input_new) + i + 1, ft_strlen(*input_new + i));
	character_store = skip_to_delimiter(*input_new + i);
	if (**input_new + i == '?' && is_delimiter_variable(*(*input_new + i + 1)))
		insert_exit_status(*input_new + i, exit_status, \
				input_length, character_store);
	else
	{
		variable = environment_variable_value_get(*input_new + i, \
				(const char **)environment);
		insert_string(input_new, variable, character_store, i);
	}
}

uint32_t	evaluate_variable(	char ***input, \
								uint32_t	i, \
								uint32_t *input_length, \
								int32_t *exit_status)
{
	char		*input_new;
	bool		in_double_quotes;
	uint32_t	split_length;
	uint32_t	capacity;

	split_length = get_split_size((const char **)*input);
	capacity = split_length + (split_length % INITIAL_TOKEN_CAPACITY) + 1;
	in_double_quotes = false;
	determine_
	input_new = ft_calloc(VARIABLE_TOKEN_SIZE \
			+ *input_length + 1, sizeof(*input_new));
	lst_memory(input_new, free, ADD);
	i = 0;
	while (input_new[i])
	{
		if (skip_single_quotes(input_new, &i, &in_double_quotes))
			return (true);
		else if (input_new[i] == '$' && (ft_isalnum(input_new[i + 1]) \
				|| input_new[i + 1] == '?' || input_new[i + 1] == '_'))
			evaluate_helper(&input_new, input_length, exit_status, i);
		else
			i++;
	}
	*input = input_new;
	*input_length = ft_strlen(*input);
	return (false);
}

uint32_t	get_spaces(char *input)
{
	uint32_t	i;
	uint32_t	counter;

	i = 0;
	counter = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			counter++;
		i++;
	}
	return (counter);
}

void	evaluate_input(	char **input[], \
						int32_t *exit_status, \
						bool *error_caught)
{
	uint32_t	input_length;
	uint32_t	i;
	uint32_t	j;
	bool		dummy;

	i = 0;
	dummy = false;
	if (!error_caught)
		error_caught = &dummy;
	/**error_caught = evaluate_variable(input, i, &input_length, \*/
	/*									exit_status);*/
	printf("%d", determine_variables(*input));
	exit (1);
	while (*input && (*input)[i] && !*error_caught)
	{
		while ((*input)[i][++j] && !*error_caught)
		{
			if ((*input)[i][j] == '\"')
				*error_caught = evaluate_double_quotes((*input)[i], &j);
			else if ((*input)[i][j] == '\'')
				*error_caught = evaluate_single_quotes((*input)[i], &j);
		}
		i++;
	}
}
