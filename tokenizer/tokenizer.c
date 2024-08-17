/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/08/17 22:41:30 by victor           ###   ########.fr       */
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

bool	unrecognized_input(char c)
{
	if (c >= 0 && c < 127 && (ft_isalnum(c) \
		|| ft_isspace(c) || is_special_char(c) \
		|| c == '?' || c == '/' || c == '-' || c == '\n' \
		|| c == ';' || c == '\'' || c == '\"' || c == '.'\
		|| c == '(' || c == ')' || c == '*' || c == '[' \
		|| c == ']' || c == '{' || c == '}' || c == '#'))
		return (false);
	p_stderr(2, "minishell: syntax error near unexpected token `%s'\n", \
			(char []){c, 0});
	return (true);
}

static uint32_t	get_word_count(char *input)
{
	uint32_t	i;
	uint32_t	word_count;

	i = 0;
	word_count = 0;
	while (input[i])
	{
		/*if (unrecognized_input(input[i]))*/
		/*	return (0);*/
		while (ft_isspace(input[i]))
			i++;
		if (is_double_special(&input[i]))
			i += 2;
		else if (is_special_char(input[i]) \
			|| input[i] == '/' || input[i] == '?' \
			|| input[i] == '-' || input[i] == '\n' \
			|| input[i] == ';' || input[i] == '\'' \
			|| input[i] == '\"' || input[i] == '*' || input[i] == '.')
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

t_token	*lexical_analysis(char *input)
{
	t_token		*tokens;
	uint32_t	i;

	if (!input || !*input || *input == '\n' || *input == ';')
		return (0);
	if (input[ft_strlen(input) - 1] == '\n')
		input[ft_strlen(input) - 1] = '\0';
	tokens = initialise_tokens(get_word_count((char *)input));
	if (!tokens)
		return (NULL);
	i = 0;
	while (*input)
	{
		while (ft_isspace(*input) || *input == ';')
		{
			if (*input == '\n' || *input == ';')
				tokens[i++].token_type = TOKEN_NEWLINE;
			*(char *)input = '\0';
			input++;
		}
		if (*input == '\0')
			break ;
		tokens[i++] = check_symbol_and_create_token((const char **)&input);
	}
	return (tokens);
}
