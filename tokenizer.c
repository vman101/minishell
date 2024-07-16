/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:24:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/16 15:16:07 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

typedef uint32_t t_symbol ;

#define LESS_THAN "<"
#define MORE_THAN ">"
#define PARANTHESIS_OPEN "("
#define PARANTHESIS_CLOSED ")"
#define SINGLE_QUOTE "\'"
#define DOUBLE_QUOTE "\""
#define AMPERSNAD "&"
#define ASTERIX	"*"
#define PIPE "|"

enum	e_symbol_types
{
	COMMAND,
	PUNCTUATION,
	OPERATOR,
	OPTION
};

typedef struct	s_token
{
	char		*value;
	t_symbol	symbol;
}				t_token;

void	tokens_destroy(void	*tokens_input)
{
	t_token		**tokens;
	uint32_t	i;

	tokens = (t_token **)tokens_input;
	i = 0;
	while (tokens[i])
	{
		ft_free((void **)&tokens[i]->value);
		ft_free((void **)&tokens[i]);
	}
	ft_free((void **)tokens);
}

void	tokenizer(const char *command_input)
{
	t_token		**tokens;
	uint32_t	split_cursor;
	uint32_t	token_cursor;
	char		**input_split;

	input_split = ft_split(command_input, ' ');
	if (!input_split)
		return (ft_putendl_fd("Failed to allocate memory for tokens", 1), lst_memory(NULL, NULL, CLEAN));
	split_cursor = 0;
	token_cursor = 0;
	tokens = ft_calloc(get_split_size((const char **)input_split) + 1, sizeof(tokens));
	lst_memory(tokens, tokens_destroy, ADD);
	while (input_split[split_cursor])
	{
		tokens[token_cursor] = ft_calloc(1, sizeof(**tokens));
		if (!tokens[token_cursor])
			return (ft_putendl_fd("Failed to allocate memory for tokens", 1), lst_memory(NULL, NULL, CLEAN));
		if (ft_strlen(input_split[split_cursor]) < 2)
			return (ft_putendl_fd("invalid input", 1));
		if (ft_memcmp(input_split[split_cursor], LESS_THAN, 2) \
			|| ft_memcmp(input_split[split_cursor], MORE_THAN, 2) \
			|| ft_memcmp(input_split[split_cursor], PIPE, 2))
			tokens[token_cursor]->symbol = OPERATOR;
		else if (ft_memcmp(input_split[split_cursor], SINGLE_QUOTE, 2) \
				|| ft_memcmp(input_split[split_cursor], DOUBLE_QUOTE, 2))
			tokens[token_cursor]->symbol = PUNCTUATION;
		else if (*input_split[split_cursor] == '-')
			tokens[token_cursor]->symbol = OPTION;
		tokens[token_cursor]->value = input_split[split_cursor];
		token_cursor++;
		split_cursor++;
	}
}
