/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_heredoc_helper.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:23:44 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 12:34:09 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	mark_tokens_till_heredoc(char *value, char **input)
{
	uint32_t	value_length;
	t_token		token;

	*(*input)++ = 0;
	token.token_value = *input;
	token.token_type = TOKEN_WORD;
	if (!value)
		return (token);
	value_length = 0;
	while (value[value_length] && !ft_isspace(value[value_length]) \
	&& (!is_special_char(value[value_length]) || value[value_length] == '$'))
		value_length++;
	remove_qoutes_delimiter(value, &value_length);
	while (**input)
	{
		if (*(*input)++ == '\n')
			if (ft_strncmp(value, *input, value_length) == 0 \
				&& (*(*input + value_length) == '\n' \
				|| *(*input + value_length) == '\0'))
				return (*input += value_length, token);
	}
	return (token);
}

void	ring_buffer_put(t_ring_buffer *buffer, t_token *entry)
{
	if (buffer->write + 1 == buffer->read)
		buffer->read++;
	buffer->buffer[buffer->write++] = entry;
	buffer->write %= buffer->buffer_capacity;
}

t_token	*ring_buffer_get(t_ring_buffer *buffer)
{
	t_token	*entry;

	if (buffer->read == buffer->write)
		return (NULL);
	entry = buffer->buffer[buffer->read++];
	buffer->read %= buffer->buffer_capacity;
	return (entry);
}

void	tokenizing_loop(char **input, \
						t_token *tokens, \
						uint32_t *i, \
						t_ring_buffer *heredoc_buffer)
{
	while (ft_isspace(**input) || **input == ';')
	{
		if (**input == '\n')
		{
			tokens[++*i].token_type = TOKEN_NEWLINE;
			while (heredoc_buffer->read != heredoc_buffer->write)
				tokens[++*i] = mark_tokens_till_heredoc(\
						ring_buffer_get(heredoc_buffer)[1].token_value, input);
		}
		else if (**input == ';')
			tokens[++(*i)].token_type = TOKEN_SEMICOLON;
		if (**input)
			*(*input)++ = 0;
	}
}
