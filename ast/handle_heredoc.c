/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:15:25 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/23 16:02:25 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_value(char *value, int fd)
{
	char	**temp;
	int		i;

	i = 0;
	if (value && value[ft_strlen(value) - 1] == '\n')
		value[ft_strlen(value) - (ft_strlen(value) > 0)] = 0;
	temp = (char **)(char *[]){value, NULL};
	evaluate_input(&temp, &i, 0);
	value = temp[0];
	ft_putendl_fd(value, fd);
}

bool	has_been_done_helper(t_token *token, char *value, \
								uint value_length, int fd)
{
	char		*token_value;
	bool		has_been_done;

	has_been_done = false;
	if (token->token_value)
	{
		token_value = token->token_value;
		token->token_type = TOKEN_DONE;
		while (token_value && *token_value)
		{
			if (*token_value++ == '\n')
			{
				if (ft_strncmp(token_value, value, value_length) == 0 \
						&& (*(token_value + value_length) == '\n' \
						|| *(token_value + value_length) == '\0'))
				{
					*token_value = 0;
					has_been_done = true;
				}
			}
		}
	}
	return (print_value(token->token_value, fd), has_been_done);
}

bool	heredoc_has_been_done(t_token *token, char *value, int fd)
{
	uint32_t	value_length;
	uint32_t	i;

	i = 0;
	while (token[i].token_type != TOKEN_NEWLINE \
			&& token[i].token_type != TOKEN_EOL)
		i++;
	while ((token[i].token_type == TOKEN_NEWLINE \
			|| token[i].token_type == TOKEN_DONE) \
			&& token[i].token_type != TOKEN_EOL)
		i++;
	value_length = ft_strlen(value);
	if (token[i].token_type == TOKEN_EOL)
		return (false);
	return (has_been_done_helper(&token[i], value, value_length, fd));
}
