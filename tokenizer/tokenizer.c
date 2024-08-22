/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/08/22 18:21:28 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

static t_token	*initialise_tokens(uint32_t word_count)
{
	t_token	*tokens;

	if (word_count == 0)
		return (NULL);
	tokens = ft_calloc(word_count + 1, sizeof(t_token));
	lst_memory(tokens, free, ADD);
	tokens[word_count].token_type = TOKEN_EOL;
	return (tokens);
}

static t_token	check_symbol_and_create_token(const char **input)
{
	if (is_double_special(*input))
		return (create_token_double_special_symbol((char **)input));
	else if (is_single_special(**input))
		return (create_token_single_special_symbol(input));
	else
		return (create_token_word(input));
}

static uint32_t	get_word_count(char *input)
{
	uint32_t	i;
	uint32_t	word_count;

	i = 0;
	word_count = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (is_double_special(&input[i]))
			i += 2;
		else if (is_in_alphabet(input[i]))
			i++;
		else if (ft_isalnum(input[i]))
			while (input[i] && !ft_isspace(input[i]) \
				&& !is_double_special(&input[i]) && !is_special_char(input[i]))
				i++;
		else if (input[i])
			i++;
		if (input[i] == '\n' || input[i] == ';')
			word_count++;
		word_count++;
	}
	return (word_count);
}

t_token	*tokens_setup(char *input)
{
	if (!input || !*input || *input == '\n' || *input == ';')
		return (NULL);
	if (input[ft_strlen(input) - 1] == '\n')
		input[ft_strlen(input) - 1] = '\0';
	return (initialise_tokens(get_word_count((char *)input)));
}

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
	remove_qoutes_delimiter(value, value_length);
	while (**input)
	{
		if (**input == '\n')
		{
			(*input)++;
			if (ft_strncmp(value, *input, value_length) == 0 \
				&& (*(*input + value_length) == '\n' || *(*input + value_length) == '\0'))
				return (*input += value_length, token);
		}
		else
			(*input)++;
	}
	return (token);
}

t_token	*lexical_analysis(char *input)
{
	t_token		*tokens;
	t_token		*heredoc_token;
	uint32_t	i;
	bool		has_heredoc;

	tokens = tokens_setup(input);
	has_heredoc = false;
	if (!tokens)
		return (NULL);
	i = -1;
	while (*input)
	{
		while (ft_isspace(*input) || *input == ';')
		{
			if (*input == '\n')
			{
				tokens[++i].token_type = TOKEN_NEWLINE;
				if (has_heredoc == true)
				{
					tokens[++i] = mark_tokens_till_heredoc(heredoc_token[1].token_value, &input);
					has_heredoc = false;
				}
			}
			else if (*input == ';')
				tokens[++i].token_type = TOKEN_SEMICOLON;
			if (*input)
				*input++ = 0;
		}
		tokens[++i] = check_symbol_and_create_token((const char **)&input);
		if (tokens[i].token_type == TOKEN_HEREDOC)
		{
			has_heredoc = true;
			heredoc_token = &tokens[i];
		}
	}
	return (tokens[++i].token_type = TOKEN_EOL, tokens);
}
