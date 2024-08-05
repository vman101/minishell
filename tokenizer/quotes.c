/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:56:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:23:10 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	determine_variables(const char *command_input)
{
	char		*tmp;
	uint32_t	i;

	tmp = ft_strchr(command_input, '$');
	i = 0;
	while (tmp)
	{
		tmp = ft_strchr(tmp + 1, '$');
		i++;
	}
	return (i);
}

void	extract_word(	char **buffer, \
						char *command_input, \
						char **variable_pointers)
{
	uint32_t	i;
	uint32_t	j;
	char		*tmp;

	*buffer = ft_calloc(get_split_length(variable_pointers) \
						+ ft_strlen(command_input) + 1, sizeof(*buffer));
	lst_memory(buffer, free, ADD, 0);
	tmp = *buffer;
	j = 0;
	while (*command_input || variable_pointers[j])
	{
		i = 0;
		while (*command_input && *command_input != '$')
			**buffer++ = *command_input++;
		if (*command_input != 0)
			command_input++;
		while (variable_pointers[j] && variable_pointers[j][i])
			*(*buffer)++ = variable_pointers[j][i++];
		while (*command_input && *command_input != ' ' && *command_input != '$')
			command_input++;
		if (variable_pointers[j])
			j++;
	}
	*buffer = tmp;
}

void	extract_variable(char **variable_pointers, \
						const char *command_input, \
						const char **environement, \
						uint32_t variable_count)
{
	char		*tmp;
	char		store_temp_eol;
	uint32_t	i;
	uint32_t	j;

	j = 0;
	tmp = (char *)command_input;
	while (j < variable_count)
	{
		i = 0;
		tmp = ft_strchr(tmp, '$');
		if (!tmp)
			return ;
		tmp++;
		while (tmp[i] && tmp[i] != '$' && tmp[i] != ' ')
			i++;
		store_temp_eol = tmp[i];
		tmp[i] = 0;
		variable_pointers[j] = environment_variable_value_get(tmp, \
														environement);
		tmp[i] = store_temp_eol;
		j++;
	}
}

char	*interpret_single_quote(const char *command_input)
{
	char	*tmp;

	tmp = ft_strchr(command_input + 1, '\'');
	if (!tmp)
		lst_memory(NULL, NULL, CLEAN, 0);
	*tmp = 0;
	tmp = (char *)command_input + 1;
	command_input = ft_strchr(tmp, '\'') + 1;
	return (tmp);
}

char	*interpret_double_quotes(	const char **command_input, \
									t_token_type *type)
{
	char	*temp_move;
	char	*return_string;

	temp_move = ft_strchr(*command_input + 1, '\"');
	if (!temp_move)
	{
		p_stderr(2, "Invalid: missing closing quote: \"\n", NULL);
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	*temp_move = 0;
	ft_memmove((char *)*command_input, *command_input + 1, ft_strlen(*command_input));
	return_string = (char *)*command_input;
	*command_input = temp_move + 1;
	*type = TOKEN_WORD;
	return (*(char **)command_input);
}
