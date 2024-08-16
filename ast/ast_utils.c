/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:43:20 by anarama           #+#    #+#             */
/*   Updated: 2024/08/16 11:36:37 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_tokens_len(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

char	**copy_args(t_ast *node, char **src)
{
	char	**temp;

	node->args = ft_calloc(get_split_size((const char **)src) \
			+ 1, sizeof(char *));
	if (!node->args)
	{
		free(node);
		perror("calloc in copy args");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(node->args, free_split, ADD);
	temp = node->args;
	while (*src)
	{
		*node->args = ft_strdup(*src);
		if (!*node->args)
		{
			free(node);
			perror("strdup in copy args");
			lst_memory(NULL, NULL, CLEAN);
		}
		src++;
		node->args++;
	}
	return (temp);
}

int	is_redirection(t_token_type	token_type)
{
	return (token_type == TOKEN_REDIRECT_IN
		|| token_type == TOKEN_REDIRECT_OUT
		|| token_type == TOKEN_REDIRECT_APPEND
		|| token_type == TOKEN_HEREDOC);
}
