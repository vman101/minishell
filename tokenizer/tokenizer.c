/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:49:35 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 17:20:46 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(void *token_ptr)
{
	t_token		*token;

	token = token_ptr;
	ft_free(&token);
}

static t_token	*initialise_tokens(uint32_t word_count)
{
	t_token	*tokens;

	tokens = ft_calloc(word_count + 1, sizeof(t_token));
	lst_memory(tokens, free_tokens, ADD, 0);
	tokens[word_count].token_type = TOKEN_EOL;
	return (tokens);
}

static t_token	check_symbol_and_create_token(const char **input,
					const char **env, int *exit_status)
{
	if (is_double_special(*input))
		return (create_token_double_special_symbol((char **)input));
	else if (is_single_special(**input))
		return (create_token_single_special_symbol(input));
	else if (is_env_var(**input))
		return (create_token_env_var((char **)input, env, exit_status));
	else if (is_quote(**input))
		return (create_token_quotes(input, env));
	else
		return (create_token_word(input, env));
}

static uint32_t	get_word_count(char *input)
{
	uint32_t	i;
	uint32_t	word_count;
	char		store_current_char;

	if (!input)
		return (0);
	i = 0;
	word_count = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (is_double_special(&input[i]))
			i += 2;
		else if (is_special_char(input[i]))
			i++;
		else if (is_quote(input[i]))
		{
			store_current_char = input[i++];
			while (input[i] && input[i] != store_current_char)
				i++;
		}
		else
		{
			while (input[i] && !ft_isspace(input[i]) \
					&& !is_double_special(&input[i]) \
					&& !is_single_special(input[i]) \
					&& !is_quote(input[i]))
				i++;
		}
		word_count++;
	}
	return (word_count);
}

t_token	*lexical_analysis(const char *input, const char **env, int
					*exit_status)
{
	t_token		*tokens;
	uint32_t	i;

	tokens = initialise_tokens(get_word_count((char *)input));
	i = 0;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (*input == '\0')
			break ;
		tokens[i++] = check_symbol_and_create_token(&input, env, exit_status);
	}
	return (tokens);
}
