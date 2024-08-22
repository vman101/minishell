/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 12:40:09 by anarama           #+#    #+#             */
/*   Updated: 2024/08/22 16:45:38 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_to_end_quotes(char **temp_move, char c)
{
	(*temp_move)++;
	while (**temp_move && **temp_move != c)
		(*temp_move)++;
	if (**temp_move)
		(*temp_move)++;
}

t_token	create_token_word(const char **input)
{
	t_token	temp_token;
	char	*temp_move;
	char	*input_next;

	temp_move = (char *)*input;
	temp_token = create_token(TOKEN_WORD, *input);
	if (**input == '$')
		temp_move++;
	while (*temp_move && !ft_isspace(*temp_move) \
		&& (!is_special_char(*temp_move) || *temp_move == '$'))
	{
		if (*temp_move == '\'' || *temp_move == '\"')
			skip_to_end_quotes(&temp_move, *temp_move);
		else
			temp_move++;
	}
	if (*temp_move && ft_isspace(*temp_move) && *temp_move != '\n')
	{
		*temp_move = 0;
		input_next = temp_move + 1;
	}
	else
		input_next = temp_move;
	*input = input_next;
	return (temp_token);
}
