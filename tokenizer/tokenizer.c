/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/08/23 14:24:32 by vvobis           ###   ########.fr       */
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
	else if (**input)
		return (create_token_word(input));
	else
		return ((t_token){0, 0});
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

t_token	*lexical_analysis(char *input)
{
	t_token			*tokens;
	uint32_t		i;
	t_ring_buffer	heredoc_buffer;

	tokens = tokens_setup(input);
	if (!tokens)
		return (NULL);
	i = -1;
	heredoc_buffer = (t_ring_buffer){0};
	heredoc_buffer.buffer_capacity = BUFFER_CAPACITY;
	while (*input)
	{
		tokenizing_loop(&input, tokens, &i, &heredoc_buffer);
		tokens[++i] = check_symbol_and_create_token((const char **)&input);
		if (tokens[i].token_type == TOKEN_HEREDOC)
			ring_buffer_put(&heredoc_buffer, &tokens[i]);
	}
	return (tokens[++i].token_type = TOKEN_EOL, tokens);
}
