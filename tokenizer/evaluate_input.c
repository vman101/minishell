/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/27 15:21:21 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*insert_exit_status(	char *input_new, \
							int32_t *exit_status, \
							uint32_t input_length)
{
	char		*exit_status_string;

	ft_memmove(input_new, input_new + 1, input_length);
	exit_status_string = ft_itoa(*exit_status);
	if (exit_status_string)
	{
		ft_memmove(input_new + ft_strlen(exit_status_string), \
				input_new, input_length);
		ft_memcpy(input_new, exit_status_string, \
				ft_strlen(exit_status_string));
	}
	ft_free(&exit_status_string);
	return (NULL);
}

bool	insert_string(	char *new_string, \
						uint32_t *i, char **environment, \
						int32_t *exit_status)
{
	char		*temp_move;
	char		*variable;
	char		character_store;
	uint32_t	length;

	temp_move = new_string;
	ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
	if (*temp_move == '?')
		return (insert_exit_status(temp_move, exit_status, \
					ft_strlen(new_string)));
	character_store = skip_to_delimiter(temp_move);
	variable = environment_variable_value_get(temp_move, \
			(const char **)environment);
	length = ft_strlen(temp_move);
	temp_move[ft_strlen(temp_move)] = character_store;
	ft_memmove(temp_move + ft_strlen(variable), \
			temp_move + length, ft_strlen(temp_move));
	ft_memcpy(temp_move, variable, ft_strlen(variable));
	*i += ft_strlen(variable) - 1;
	return (true);
}

char	*evaluate_helper(char *string_to_change, \
						int32_t *exit_status)
{
	bool	in_double_quotes;
	char	*new_string;
	uint	i;

	in_double_quotes = false;
	i = 0;
	new_string = NULL;
	while (string_to_change[i])
	{
		if (skip_single_quotes(string_to_change, &i, &in_double_quotes))
			return (NULL);
		if (string_to_change[i] == '$' && \
				!is_delimiter_variable(string_to_change[i + 1]))
		{
			new_string = ft_calloc(determine_full_length(string_to_change, \
						env_static(NULL)) + 1, sizeof(*new_string));
			lst_memory(new_string, free, ADD);
			ft_memcpy(new_string, string_to_change, \
					ft_strlen(string_to_change));
			insert_string(&new_string[i], &i, env_static(NULL), exit_status);
			string_to_change = new_string;
		}
		i++;
	}
	return (new_string);
}

char	**determine_new_size(char **input)
{
	uint32_t	split_length;
	uint32_t	capacity;
	uint32_t	new_size;
	char		**input_new;

	split_length = get_split_size((const char **)input);
	capacity = split_length + (INITIAL_TOKEN_CAPACITY - \
			(split_length % INITIAL_TOKEN_CAPACITY));
	new_size = determine_variables(input);
	if (new_size >= capacity - 1)
	{
		new_size = new_size + (INITIAL_TOKEN_CAPACITY - \
				(new_size % INITIAL_TOKEN_CAPACITY));
		input_new = ft_calloc(new_size, sizeof(*input_new));
		lst_memory(input_new, free, ADD);
		ft_memcpy(input_new, *input, split_length);
		return (input_new);
	}
	return (input);
}

uint32_t	evaluate_variable(	char ***input, \
								int32_t *exit_status)
{
	char		*new_string;
	uint32_t	i;

	*input = determine_new_size(*input);
	i = 0;
	while ((*input)[i])
	{
		if (ft_strchr((*input)[i], '$'))
		{
			new_string = evaluate_helper((*input)[i], exit_status);
			if (new_string)
				adjust_input(&(*input)[i], new_string);
		}
		i++;
	}
	return (false);
}
