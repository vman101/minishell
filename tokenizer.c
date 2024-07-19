/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:24:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/19 15:53:11 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <stdint.h>

#define BUFFER_SIZE 2048

#define TOKENS_MAX_AMOUNT 1024

char	*input_extract_word(char *command_input, uint32_t *i)
{
	uint32_t	j;

	j = 0;
	while (ft_isprint(command_input[j]) && !is_operator(command_input[j]))
		j++;
	*i += j;
	return (ft_substr(command_input, 0, j));
}

void	token_destroy(void *token_input)
{
	t_token		**tokens;
	uint32_t	i;

	tokens = (t_token **)token_input;
	i = 0;
	while (tokens[i])
	{
		ft_free((void **)&tokens[i]->value);
		ft_free((void **)&tokens[i]);
		i++;
	}
	ft_free((void **)&tokens);
}

t_token	*token_create(char *value, t_symbol symbol)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(*token));
	token->value = value;
	token->symbol = symbol;
	return (token);
}

void	tokenizer_strtrim(char *string, char to_trim)
{
	uint32_t	i;
	uint32_t	string_length;

	i = 0;
	string_length = ft_strlen(string);
	while (string[i] && string[i] == to_trim)
		i++;
	ft_memmove(string, string + i, ft_strlen(string + i) + 1);
	i = string_length;
	while (i && string[i] == to_trim)
		i--;
	string[i] = 0;
}

t_token	**tokenizer(char *command_input, const char **environement)
{
	uint32_t	i;
	uint32_t	k;
	t_token		**tokens;

	i = 0;
	k = 0;
	tokens = ft_calloc(TOKENS_MAX_AMOUNT, sizeof(tokens));
	lst_memory(tokens, token_destroy, ADD);
	while (command_input[i])
	{
		if (command_input[i] == '\"')
			tokens[k] = token_create(interpret_double_quotes(&command_input[i + 1], \
													environement, &i), TOKEN_STRING_LITERAL);
		else if (command_input[i] == '\'')
			tokens[k] = token_create(interpret_single_quote(&command_input[i], &i), TOKEN_STRING_LITERAL);
		else if (is_operator(command_input[i]))
			tokens[k] = find_operator(&command_input[i], &i);
		else if (ft_isalpha(command_input[i]))
			tokens[k] = token_create(input_extract_word(&command_input[i], &i), TOKEN_WORD);
		else if (command_input[i] == '$')
		{
			tokens[k] = token_create(extract_variable(&command_input[i++], environement), TOKEN_STRING_LITERAL);
			if (tokens[k]->value)
				i += ft_strlen(tokens[k]->value);
		}
		else
			i++;
		while (command_input[i] == ' ')
			i++;
		if (tokens[k]->value)
			tokenizer_strtrim(tokens[k]->value, ' ');
		k++;
		if (k == TOKENS_MAX_AMOUNT)
			return (lst_memory(tokens, NULL, FREE), \
					ft_putendl_fd("Too Many tokens! (Max = 1024)", 1), NULL);
	}
	return (tokens);
}
