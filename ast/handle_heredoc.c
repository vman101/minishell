/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 11:15:25 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/21 12:05:00 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_heredoc(t_token *tokens, int32_t pipefd)
{
	char		*value;
	char		*tmp;
	char		*tmp2;
	char		character_store;
	uint32_t	value_length;
	uint32_t	token_length;

	value = tokens[0].token_value;
	tmp = value;
	while (*tmp && ft_isalnum(*tmp))
		tmp++;
	character_store = *tmp;
	*tmp = 0;
	value_length = ft_strlen(value);
	*tmp = character_store;
	tmp = ft_strchr(value, '\n');
	if (tmp)
	{
		*tmp++ = 0;
		while (tmp)
		{
			tmp2 = tmp;
			tmp = ft_strchr(tmp2, '\n');
			if (tmp)
				*tmp++ = 0;
			token_length = ft_strlen(tmp2);
			if (token_length >= value_length)
				if (ft_memcmp(value, tmp2, token_length) == 0)
					break ;
			ft_putendl_fd(tmp2, pipefd);
		}
	}
	tokens->token_type = TOKEN_DONE;
}

static bool	heredoc_has_been_done_helper(	char *value, \
											char *tmp, \
											char character_store)
{
	char	*tmp2;

	while (tmp && *tmp)
	{
		tmp2 = tmp + 1;
		tmp = ft_strchr(tmp + 1, '\n');
		if (tmp)
			*tmp = 0;
		if (ft_memcmp(value, tmp2, ft_strlen(value)) == 0)
		{
			if (tmp)
				*tmp = '\n';
			*ft_strchr(value, 0) = character_store;
			return (true);
		}
		if (tmp)
			*tmp = '\n';
	}
	return (false);
}

bool	heredoc_has_been_done(char *value)
{
	char	*tmp;
	char	character_store;

	tmp = value;
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	character_store = *tmp;
	if (ft_isspace(*tmp) && *tmp != '\n')
	{
		*tmp = 0;
		tmp = ft_strchr(tmp, '\n');
		tmp += (tmp != 0);
	}
	else
		*tmp = 0;
	return (heredoc_has_been_done_helper(value, tmp, character_store));
}

