/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:56:11 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/17 18:24:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint32_t	determine_variables(char *command_input)
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

static char	*extract_word(char *command_input, char **variable_pointers)
{
	char		*buffer;
	char		*tmp;
	char		*tmp2;
	uint32_t	j;

	buffer = ft_calloc(get_split_length(variable_pointers) \
						+ ft_strlen(command_input) + 1, \
						sizeof(*buffer));
	if (!buffer)
		lst_memory(NULL, NULL, CLEAN);
	j = 0;
	tmp2 = command_input;
	tmp = command_input;
	while (variable_pointers[j])
	{
		tmp2 = ft_strchr(tmp2, '$');
		ft_strlcpy(buffer + ft_strlen(buffer), tmp, tmp2++ - tmp + 1);
		ft_strlcpy(buffer + ft_strlen(buffer), variable_pointers[j], \
					ft_strlen(variable_pointers[j]) + 1);
		tmp = ft_strchr(tmp2, ' ');
		j++;
	}
	free_split(variable_pointers);
	return (buffer);
}

char	*extract_variable(char *command_input, \
							const char **environement)
{
	char		*variable_pointers;
	char		*tmp;
	char		*move;

	tmp = ft_strchr(command_input, '$');
	move = ft_strchr(tmp, ' ');
	if (move)
		*move = 0;
	variable_pointers = environment_variable_get(tmp + 1, environement);
	if (variable_pointers == 0)
		variable_pointers = ft_strdup("");
	if (move)
		*move = ' ';
	return (variable_pointers);
}

char	*interpret_single_quote(char *command_input, uint32_t *i)
{
	char	*tmp;

	tmp = ft_strchr(command_input + 1, '\'');
	if (!tmp)
		lst_memory(NULL, NULL, CLEAN);
	*i += tmp - command_input + 1;
	return (ft_substr(command_input + 1, 0, tmp - command_input - 1));
}

char	*interpret_double_quotes(	char *command_input, \
									const char **environement, \
									uint32_t *i)
{
	char		**variable_pointers;
	char		*buffer;
	char		*tmp;
	uint32_t	variable_count;
	uint32_t	j;

	variable_count = determine_variables(command_input);
	variable_pointers = ft_calloc(variable_count, sizeof(variable_count));
	if (!variable_pointers)
		lst_memory(NULL, NULL, CLEAN);
	tmp = ft_strchr(command_input + 1, '\"');
	if (!tmp)
		lst_memory(NULL, NULL, CLEAN);
	j = 0;
	*tmp = 0;
	if (variable_count)
	{
		while (j < variable_count)
			variable_pointers[j++] = extract_variable(command_input, environement);
		buffer = extract_word(command_input, variable_pointers);
	}
	else
		buffer = ft_strdup(command_input);
	tmp = ft_strchr(command_input, '\0');
	*tmp = ' ';
	*i += tmp - command_input;
	return (buffer);
}
