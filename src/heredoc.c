/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:18:42 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/30 11:08:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

void	*ft_realloc(void *ptr, int old_size, int new_size)
{
	void	*new_ptr;

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
	{
		new_ptr = ft_calloc(new_size, 1);
		return (new_ptr);
	}
	new_ptr = ft_calloc(new_size, 1);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	return (new_ptr);
}

int	get_amount_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

uint32_t	get_input_amount(char *input)
{
	uint32_t	i;

	i = 0;
	if (!input)
		return (0);
	while (*input)
	{
		if (*input == '\n')
			i++;
		input++;
	}
	return (i);
}

void	fill_new_tokens(t_token **tokens, t_token **tokens_old, char *input)
{
	uint32_t	i;
	uint32_t	j;
	char		*temp_move;

	i = 0;
	while (tokens[i]->token_type != TOKEN_HEREDOC)
	{
		tokens[i] = tokens_old[i];
		i++;
	}
}

void	hanlde_heredoc(t_token **tokens, const char **environment)
{
	int		i;
	t_token	*new_tokens;
	t_token	*tmp;
	char	**heredoc;

	i = 0;
	while ((*tokens)[i].token_type != TOKEN_EOL)
	{
		if ((*tokens)[i].token_type == TOKEN_HEREDOC)
		{
			token_heredoc_get(tokens[i], tokens[i + 1]->token_value, environment);
			new_tokens = (t_token *)ft_calloc(get_amount_tokens(*tokens) + get_input_amount(tokens[i]->token_value) + 1, sizeof(t_token));
			ft_memcpy(new_tokens, *tokens, i * sizeof(t_token));
			tmp = new_tokens;
			while (tmp->token_type != TOKEN_EOL)
				tmp++;
			tmp->token_type = TOKEN_WORD;
			tmp->token_value = ft_strdup("heredoc");
			tmp++;
			ft_memcpy(tmp, *tokens + i + 1,
				(get_amount_tokens(*tokens) - i) * sizeof(t_token));
			free(*tokens);
			*tokens = new_tokens;
			return ;
		}
		i++;
	}
}