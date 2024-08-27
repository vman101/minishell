/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:10:37 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/27 15:20:08 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	get_spaces(char *input)
{
	uint32_t	i;
	uint32_t	counter;
	char		character_store;

	i = 0;
	counter = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			character_store = input[i];
			while (input[i] && input[i] != character_store)
				i++;
		}
		if (input[i] == ' ')
			counter++;
		i++;
	}
	return (counter);
}

uint32_t	determine_variables(char **input)
{
	uint	i;
	uint	j;
	uint	counter;
	char	*tmp;
	char	character_store;

	i = -1;
	counter = 1;
	while (input[++i])
	{
		j = -1;
		while (input[i][++j])
		{
			if (input[i][j] == '$')
			{
				character_store = skip_to_delimiter(&input[i][j + 1]);
				tmp = environment_variable_value_get(&input[i][j + 1], \
						(const char **)env_static(NULL));
				if (tmp)
					counter += get_spaces(tmp) + 1;
				input[i][ft_strlen(input[i])] = character_store;
			}
		}
	}
	return (counter);
}

void	split_string_by_space(char *to_split, char **input_new)
{
	uint32_t	i;
	char		character_store;

	i = 0;
	input_new[i++] = to_split;
	while (*to_split)
	{
		if (*to_split == '\"' || *to_split == '\'')
		{
			character_store = *to_split++;
			while (*to_split && *to_split != character_store)
				to_split++;
		}
		if (*to_split == ' ')
		{
			*to_split = 0;
			input_new[i++] = to_split + 1;
		}
		to_split++;
	}
}

void	adjust_input(char **input, char *new_string)
{
	uint32_t	count;

	count = get_spaces(new_string) + 1;
	if (count)
		ft_memmove(input + count, input + 1, \
				get_split_size((const char **)input) * sizeof(*input));
	split_string_by_space(new_string, input);
}

uint32_t	determine_full_length(char *string, char **environment)
{
	uint32_t	size;
	uint32_t	i;
	char		character_store;

	size = 0;
	i = 0;
	character_store = 0;
	while (string[i])
	{
		if (string[i] == '$')
		{
			character_store = skip_to_delimiter(&string[i + 1]);
			if (string[i] == '?')
				size += 4;
			else
				size += ft_strlen(environment_variable_value_get(\
							&string[i + 1], (const char **)environment));
			while (string[i])
				i++;
			string[i] = character_store;
		}
		else
			i++;
	}
	return (size + i);
}
