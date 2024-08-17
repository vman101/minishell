/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:40:58 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/17 22:19:06 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_string(	char *input_new, \
						char *string_to_insert, \
						char character_store)
{
	char		*temp_move;
	uint32_t	string_to_insert_length;
	uint32_t	input_length;

	input_length = ft_strlen(input_new);
	temp_move = ft_strchr(input_new, 0);
	*temp_move = character_store;
	if (!string_to_insert)
	{
		ft_bzero(input_new, input_length);
		ft_memmove(input_new, temp_move, ft_strlen(temp_move) + 1);
		return ;
	}
	string_to_insert_length = ft_strlen(string_to_insert);
	ft_memmove(input_new + string_to_insert_length, temp_move, ft_strlen(temp_move) + 1);
	ft_memcpy(input_new, string_to_insert, string_to_insert_length);
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

bool	is_delimiter(char c)
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

void	evaluate_helper(char *input_new, uint32_t *input_length, \
						int32_t *exit_status)
{
	char	character_store;
	char	**environment;
	char	*variable;

	environment = env_static(NULL);
	ft_memmove(input_new, input_new + 1, ft_strlen(input_new));
	character_store = skip_to_delimiter(input_new);
	if (*input_new == '?' && is_delimiter(*(input_new + 1)))
		insert_exit_status(input_new, exit_status, \
				input_length, character_store);
	else
	{
		variable = environment_variable_value_get(input_new, (const char **)environment);
		insert_string(input_new, variable, character_store);
	}
}

uint32_t	evaluate_variable(	char **input, \
								uint32_t *input_length, \
								int32_t *exit_status)
{
	char		*input_new;
	uint32_t	i;
	bool		in_double_quotes;

	in_double_quotes = false;
	input_new = ft_calloc(VARIABLE_TOKEN_SIZE + 1, sizeof(*input_new));
	lst_memory(input_new, free, ADD);
	ft_strlcpy(input_new, *input, *input_length + 1);
	i = 0;
	while (input_new[i])
	{
		if (skip_single_quotes(input_new, &i, &in_double_quotes))
			return (true);
		else if (input_new[i] == '$' && (ft_isalnum(input_new[i + 1]) || input_new[i + 1] == '?'))
			evaluate_helper(input_new + i, input_length, exit_status);
		else
			i++;
	}
	*input = input_new;
	*input_length = ft_strlen(*input);
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
	while ((*input)[i] && !*error_caught)
	{
		input_length = ft_strlen((*input)[i]);
		j = 0;
		if (ft_strchr((*input)[i], '$'))
			*error_caught = evaluate_variable(&(*input)[i], &input_length, \
												exit_status);
		while ((*input)[i][j] && !*error_caught)
		{
			if ((*input)[i][j] == '\"')
				*error_caught = evaluate_double_quotes((*input)[i], &j);
			else if ((*input)[i][j] == '\'')
				*error_caught = evaluate_single_quotes((*input)[i], &j);
			j++;
		}
		i++;
	}
}
