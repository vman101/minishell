/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/10 22:30:20 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdint.h>
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

void	token_heredoc_get(t_token *token, const char *delimiter, const char **environment)
{
	static t_prompt	heredoc = {0};

	if (heredoc.exists == false)
    	heredoc = prompt_create(environment, CUSTOM);
	heredoc.prompt_length = prompt_display_string_set(&heredoc, \
													NULL, \
													"heredoc> ");
	token->token_value = token_heredoc_input_get(&heredoc, delimiter);
	token->token_type = TOKEN_WORD;
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

void heredoc_while_tokenizing(char *input)
{
	char		*delimiter;
	char		*temp_move;
	char		*temp_cut;
	uint32_t	length;
	char		character_store;
	uint32_t	delimiter_length;

	input += 2;
	if (!is_mutliple_lines(input))
		return ;
	delimiter = input;
	while (ft_isspace(*delimiter))
		delimiter++;
	temp_move = delimiter;
	while (*temp_move && !ft_isspace(*temp_move) && !is_single_special(*temp_move))
		temp_move++;
	character_store = *temp_move;
	*temp_move = 0;
	delimiter_length = ft_strlen(delimiter);
	*temp_move = character_store;
	input = temp_move;
	temp_move = ft_strchr(input, '\n');
	if (temp_move)
	{
		while (temp_move && *temp_move)
		{
			*temp_move = ' ';
			temp_cut = temp_move;
			while (*temp_cut && *temp_cut != '\n')
				temp_cut++;
			length = temp_cut - temp_move;
			while (length--)
			{
				ft_memmove(input + 1, input, ft_strlen(input));
				*input++ = *++temp_move;
				ft_memmove(temp_move, temp_move + 1, ft_strlen(temp_move));
			}
			if (length >= delimiter_length)
				if (ft_strncmp(input - delimiter_length, delimiter, delimiter_length) == 0)
					break ;
			temp_move = ft_strchr(temp_move, '\n');
		}
	}
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
		heredoc_while_tokenizing(*input);
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
