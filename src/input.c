/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:18:42 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/19 22:58:56 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_split(void *back)
{
	char	**free_back;

	if (!back)
		return ;
	free_back = (char **)back;
	while (*free_back)
	{
		ft_free((void *)free_back);
		free_back++;
	}
	ft_free(&back);
	return ;
}

uint32_t	get_split_length(char **split)
{
	uint32_t	i;
	uint32_t	k;
	uint32_t	j;

	i = 0;
	k = 0;
	while (split[i])
	{
		j = 0;
		while (split[i][j++])
			k++;
		i++;
	}
	return (k);
}

uint32_t	get_split_size(const char **split)
{
	uint32_t	i;

	i = 0;
	if (!split)
		return (0);
	while (split[i])
		i++;
	return (i);
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
