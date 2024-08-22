/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:15:25 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/22 17:38:48 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	heredoc_has_been_done(t_token *token, char *value, int32_t fd)
{
	char		*token_value;
	uint32_t	value_length;
	uint32_t	i;

	i = 0;
	while (token[i].token_type != TOKEN_NEWLINE && token[i].token_type != TOKEN_EOL)
		i++;
	value_length = ft_strlen(value);
	if (token[i].token_type == TOKEN_EOL)
		return (false);
	if (token[++i].token_value)
	{
		token_value = token[i].token_value;
		ft_putstr_fd(token[i].token_value, fd); 
		token[i].token_type = TOKEN_DONE;
		while (token_value && *token_value)
		{
			if (*token_value == '\n')
			{
				token_value++;
				if (ft_strncmp(token_value, value, value_length) == 0 \
						&& (*(token_value + value_length) == '\n' || *(token_value + value_length) == '\0'))
					return (true);
			}
			else
				token_value++;
		}
	}
	return (false);
}
