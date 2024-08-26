/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:36:35 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 14:22:12 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

uint32_t	determine_trees(t_token *tokens)
{
	uint32_t	tree_count;
	uint32_t	i;
	bool		has_heredoc;

	i = 0;
	tree_count = 1;
	has_heredoc = false;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		if (tokens[i].token_type == TOKEN_HEREDOC)
			has_heredoc = true;
		if (is_delimiter_token(&tokens[i]) || \
				(tokens[i].token_type == TOKEN_NEWLINE && has_heredoc == true))
		{
			has_heredoc = false;
			tree_count++;
		}
		i++;
	}
	return (tree_count);
}

void	tree_destroy(void *tree_ptr)
{
	uint32_t	i;
	t_ast		*tree;

	i = 0;
	tree = (t_ast *)tree_ptr;
	while (tree && tree[i].type != NODE_END)
	{
		if (tree[i].has_redir_out == true)
			ft_close(tree[i].fd_out, "fd_out in tree_destroy");
		if (tree[i].is_heredoc)
			ft_free(&tree->path_file_in);
		if (tree[i].args)
			ft_free(&tree[i].args);
		i++;
	}
	ft_free(&tree);
}

bool	is_delimiter_token(t_token *token)
{
	return (token->token_type == TOKEN_EOL \
			|| token->token_type == TOKEN_AND \
			|| token->token_type == TOKEN_OR \
			|| token->token_type == TOKEN_NEWLINE \
			|| token->token_type == TOKEN_PIPE \
			|| token->token_type == TOKEN_SEMICOLON);
}
