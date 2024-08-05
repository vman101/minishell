/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_env_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:37:49 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 17:37:13 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_env_var(const char input)
{
	return (input == '$');
}

void	variable_expand(char **input, \
						char **buffer, \
						const char **environement, \
						char *temp_move)
{
	uint32_t	variable_count;
	char		**variable_pointers;
	char		store_current_char;

	store_current_char = 0;
	variable_count = determine_variables(*input);
	variable_pointers = ft_calloc(variable_count + 1, sizeof(char *));
	if (!variable_pointers)
		return (lst_memory(NULL, NULL, CLEAN, 0));
	while (*temp_move && !is_special_char(*temp_move) && *temp_move != '$')
		temp_move++;
	if (*temp_move != ' ')
		store_current_char = *temp_move;
	*temp_move = 0;
	extract_variable(variable_pointers, *input, environement, variable_count);
	extract_word(buffer, (char *)*input, variable_pointers);
	if (temp_move)
		*input = temp_move;
	else
		*input = ft_strchr(*input, 0);
	*temp_move = store_current_char;
	ft_free(&variable_pointers);
}

t_token	create_token_env_var(char **input, const char **environement,
						int *exit_status)
{
	char		*buffer;
	char		*temp_move;

	buffer = NULL;
	temp_move = *input + 1;
	if (*temp_move == '(')
	{
		*input = temp_move + 1;
		temp_move = ft_strrchr(*input, ')');
		if (!temp_move)
			lst_memory(NULL, NULL, CLEAN, 0);
		*temp_move = 0;
		buffer = execute_subshell(*input, environement);
		*input = temp_move + 1;
	}
	else if (*temp_move == '?')
	{
		buffer = ft_itoa(*exit_status);
		lst_memory(buffer, free, ADD, 0);
		*input =  temp_move + 1;
	}
	else
	{
		variable_expand(input, &buffer, environement, temp_move);
	}
	return (create_token(TOKEN_ENV, buffer));
}
