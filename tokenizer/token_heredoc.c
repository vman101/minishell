/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:44:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 17:50:28 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

static char	*token_heredoc_input_get(t_prompt *heredoc, const char *delimiter)
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
							const char *delimiter)
{
	static t_prompt	heredoc = {0};

	if (heredoc.exists == false)
	{
		heredoc = prompt_create(CUSTOM);
		heredoc.prompt_length = prompt_display_string_set(&heredoc, \
				NULL, \
				"heredoc> ");
	}
	token->token_value = token_heredoc_input_get(&heredoc, delimiter);
	token->token_type = TOKEN_DONE;
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

char	*handle_delimiter(char *input, char **temp_move)
{
	char		*delimiter;
	uint32_t	delimiter_length;

	delimiter = input;
	while (ft_isspace(*delimiter))
		delimiter++;
	delimiter_length = ft_strlen(delimiter);
	remove_qoutes_delimiter(delimiter, &delimiter_length);
	*temp_move = delimiter;
	while (*temp_move && !ft_isspace(**temp_move) \
		&& !is_single_special(**temp_move))
	{
		if (**temp_move == '\'' || **temp_move == '\"')
			ft_memmove(*temp_move, *temp_move + 1, ft_strlen(*temp_move));
		else
			(*temp_move)++;
	}
	return (delimiter);
}

char	*heredoc_while_tokenizing(char *input)
{
	char		*delimiter;
	char		*temp_move;
	char		character_store;
	uint32_t	delimiter_length;

	if (!is_mutliple_lines(input))
		return (input);
	delimiter = handle_delimiter(input, &temp_move);
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
