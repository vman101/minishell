/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/19 19:45:35 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int	is_double_special(const char *input)
{
	return ((ft_strncmp(input, ">>", 2) == 0)
		|| (ft_strncmp(input, "<<", 2) == 0)
		|| (ft_strncmp(input, "&&", 2) == 0)
		|| (ft_strncmp(input, "||", 2) == 0));
}

char	*token_heredoc_input_get(t_prompt *heredoc, const char *delimiter)
{
	char	*heredoc_input;

	heredoc_input = prompt_get_input(heredoc, \
									PROMPT_INPUT_BUFFER_SIZE, \
									delimiter);
	if (!heredoc_input || !*heredoc_input)
	{
		if (g_signal_flag == 2 || g_signal_flag == 1)
			g_signal_flag = 0;
		return (NULL);
	}
	return (heredoc_input);
}

void	token_heredoc_get(	t_token *token, \
							const char *delimiter, \
							const char **environment)
{
	static t_prompt	heredoc = {0};

	if (heredoc.exists == false)
	{
		heredoc = prompt_create(environment, CUSTOM);
		heredoc.prompt_length = prompt_display_string_set(&heredoc, \
				NULL, \
				"heredoc> ");
	}
	token->token_value = token_heredoc_input_get(&heredoc, delimiter);
	token->token_type = TOKEN_DONE;
}

bool	is_mutliple_lines(char *c)
{
	while (*c)
	{
		if (*c == '\n')
			return (true);
		c++;
	}
	return (false);
}

char	*heredoc_loop(	char *input, \
						char *temp_move, \
						const char *delimiter, \
						uint32_t delimiter_length)
{
	char		*temp_cut;
	uint32_t	length;
	uint32_t	i;

	while (temp_move && *temp_move)
	{
		temp_cut = temp_move + 1;
		while (*temp_cut && *temp_cut != '\n')
			temp_cut++;
		length = temp_cut - temp_move;
		i = 0;
		while (i < length)
		{
			ft_memmove(input + 1, input, ft_strlen(input));
			*input++ = *++temp_move;
			ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
			i++;
		}
		if (length >= delimiter_length)
			if (ft_memcmp(input - length + 1, \
				delimiter, length - 1) == 0)
				break ;
		temp_move = ft_strchr(temp_move, '\n');
	}
	return (input);
}

void	remove_qoutes_delimiter(char *delimiter)
{
	uint32_t	length;
	char		quote;

	length = ft_strlen(delimiter);
	if (*delimiter == '\"' || *delimiter == '\'')
		quote = *delimiter;
	else
		return ;
	ft_memmove(delimiter, delimiter + 1, length);
	delimiter = ft_strchr(delimiter, quote);
	if (delimiter)
		ft_memmove(delimiter, delimiter + 1, ft_strlen(delimiter));
}

char	*heredoc_while_tokenizing(char *input)
{
	char		*delimiter;
	char		*temp_move;
	char		character_store;
	uint32_t	delimiter_length;

	input += 2;
	if (!is_mutliple_lines(input))
		return (input);
	delimiter = input;
	while (ft_isspace(*delimiter))
		delimiter++;
	remove_qoutes_delimiter(delimiter);
	temp_move = delimiter;
	while (*temp_move && !ft_isspace(*temp_move) \
		&& !is_single_special(*temp_move))
	{
		if (*temp_move == '\'' || *temp_move == '\"')
			ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
		else
			temp_move++;
	}
	character_store = *temp_move;
	*temp_move = 0;
	delimiter_length = ft_strlen(delimiter);
	*temp_move = character_store;
	input = temp_move + (*temp_move != 0);
	temp_move = input;
	if (temp_move)
		return (heredoc_loop(input, temp_move, delimiter, delimiter_length));
	return (input);
}

t_token	create_token_double_special_symbol(char **input)
{
	t_token			temp_token;
	t_token_type	token_type;

	if (ft_strncmp(*input, ">>", 2) == 0)
		token_type = TOKEN_REDIRECT_APPEND;
	else if (ft_strncmp(*input, "<<", 2) == 0)
	{
		token_type = TOKEN_HEREDOC;
		*input += 2;
		while (**input && **input == ' ')
			(*input)++;
		temp_token = create_token(token_type, *input);
		if (!isatty(0))
		{
			*input = heredoc_while_tokenizing(*input);
			**input = 0;
			(*input)++;
		}
		else
		{
			while (**input && !ft_isspace(**input) && !is_special_char(**input))
				(*input)++;
			*(*input++) = 0;
		}
		return (temp_token);
	}
	else if (ft_strncmp(*input, "&&", 2) == 0)
		token_type = TOKEN_AND;
	else
		token_type = TOKEN_OR;
	temp_token = create_token(token_type, NULL);
	**input = 0;
	*input += 2;
	return (temp_token);
}
