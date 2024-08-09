/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/02 17:43:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	char	*tmp;

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
	char			*temp_move;

	if (heredoc.exists == false)
    	heredoc = prompt_create(environment, CUSTOM);
	heredoc.prompt_length = prompt_display_string_set(&heredoc, \
													NULL, \
													"heredoc> ");
	token->token_value = token_heredoc_input_get(&heredoc, delimiter);
	token->token_type = TOKEN_WORD;
}

t_token	create_token_double_special_symbol(char **input, const char **environment)
{
	t_token			temp_token;
	t_token_type	token_type;

	if (ft_strncmp(*input, ">>", 2) == 0)
		token_type = TOKEN_REDIRECT_APPEND;
	else if (ft_strncmp(*input, "<<", 2) == 0)
	   token_type = TOKEN_HEREDOC;
	else if (ft_strncmp(*input, "&&", 2) == 0)
		token_type = TOKEN_AND;
	else
		token_type = TOKEN_OR;
	temp_token = create_token(token_type, NULL);
	**input = 0;
	*input += 2;
	return (temp_token);
}
