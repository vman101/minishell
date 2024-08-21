/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_double_special_symbol.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:35:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/21 15:31:52 by vvobis           ###   ########.fr       */
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

bool	is_mutliple_lines(char *c)
{
	bool	has_mulitple_lines;

	has_mulitple_lines = false;
	while (*c)
	{
		if (*c == '\n' && !has_mulitple_lines)
			has_mulitple_lines = true;
		else if (*c == '\n' && has_mulitple_lines)
			return (true);
		c++;
	}
	return (false);
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

t_token	create_token_heredoc(char **input)
{
	t_token	temp_token;

	*input += 2;
	while (**input && **input == ' ' && !is_special_char(*((*input)+ 1)))
		(*input)++;
	temp_token = create_token(TOKEN_HEREDOC, *input);
	if (!isatty(0))
	{
		*input = heredoc_while_tokenizing(*input);
		**input = 0;
		(*input)++;
	}
	else
	{
		while (**input && !ft_isspace(**input) && !is_special_char(*((*input) + 1)))
			(*input)++;
		if (**input)
			**input = 0;
		(*input)++;
	}
	return (temp_token);
}

t_token	create_token_double_special_symbol(char **input)
{
	t_token			temp_token;
	t_token_type	token_type;

	if (ft_strncmp(*input, ">>", 2) == 0)
		token_type = TOKEN_REDIRECT_APPEND;
	else if (ft_strncmp(*input, "<<", 2) == 0)
		return (create_token_heredoc(input));
	else if (ft_strncmp(*input, "&&", 2) == 0)
		token_type = TOKEN_AND;
	else
		token_type = TOKEN_OR;
	temp_token = create_token(token_type, NULL);
	**input = 0;
	*input += 2;
	return (temp_token);
}
