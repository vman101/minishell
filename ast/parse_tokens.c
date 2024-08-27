/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:46:26 by anarama           #+#    #+#             */
/*   Updated: 2024/08/27 15:04:45 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	parse_branch(t_token *tokens, t_ast *branch)
{
	int			capacity;
	int			count;
	uint32_t	i;

	count = 0;
	capacity = INITIAL_TOKEN_CAPACITY;
	branch->args = ft_calloc(capacity + 1, sizeof(char *));
	if (!branch->args)
		return (perror("calloc in parse tokens"), \
				lst_memory(NULL, NULL, CLEAN));
	i = -1;
	while (!is_delimiter_token(&tokens[++i]))
	{
		if (tokens[i].token_type != TOKEN_DONE)
		{
			fill_args(&branch->args, count++, tokens[i].token_value, &capacity);
			tokens[i].token_type = TOKEN_DONE;
		}
	}
	if (tokens[i].token_type == TOKEN_EOL)
		return ;
	if (branch->type != NODE_INVALID)
		branch->type = (t_node_type)tokens[i].token_type;
	tokens[i].token_type = TOKEN_DONE;
}

static t_ast	collect_redirection(t_token *token, \
									int has_syntax_error)
{
	uint32_t	i;
	t_ast		branch;

	i = 0;
	branch = (t_ast){0};
	branch.fd_in = STDIN_FILENO;
	branch.fd_out = STDOUT_FILENO;
	while (!is_delimiter_token(&token[i]) && branch.type != NODE_INVALID)
	{
		if (token[i + 1].token_type == TOKEN_SEMICOLON)
			token[i + 1].token_type = TOKEN_NEWLINE;
		if (token[i + 1].token_type == TOKEN_WORD)
		{
			if (!has_syntax_error)
			{
				handle_redir_in(&branch, &token[i], &token[i + 1]);
				handle_redir_out(&branch, &token[i], &token[i + 1]);
				handle_redir_append(&branch, &token[i], &token[i + 1]);
			}
		}
		if (has_syntax_error == 0)
			handle_redir_heredoc(&branch, &token[i], i);
		i++;
	}
	return (branch);
}

void	check_syntax_errors(t_token *token, int *error_catched)
{
	int	i;

	i = 0;
	while (token[i].token_type != TOKEN_NEWLINE \
			&& token[i].token_type != TOKEN_EOL \
			&& token[i].token_type != TOKEN_SEMICOLON \
			&& *error_catched == 0)
	{
		if (token[i].token_type == TOKEN_REDIRECT_IN \
			|| token[i].token_type == TOKEN_REDIRECT_OUT \
			|| token[i].token_type == TOKEN_REDIRECT_APPEND \
			|| token[i].token_type == TOKEN_HEREDOC)
			check_valid_redir(token, i, error_catched);
		else if (token[i].token_type == TOKEN_PIPE)
			check_valid_pipe(token, i, error_catched);
		else if (token[i].token_type == TOKEN_AND \
				|| token[i].token_type == TOKEN_OR)
			check_valid_logical_operator(token, i, error_catched);
		i++;
	}
}

t_ast	*parse_tokens(	t_token *tokens, \
						int32_t *exit_status)
{
	t_ast		*tree;
	int			i;
	uint32_t	tree_count;
	int			has_syntax_error;

	if (!tokens)
		return (NULL);
	i = 0;
	tree_count = determine_trees(tokens);
	tree = ft_calloc(tree_count + 1, sizeof(t_ast));
	lst_memory(tree, tree_destroy, ADD);
	tree[tree_count].type = NODE_END;
	has_syntax_error = false;
	while (tree[i].type != NODE_END && !has_syntax_error)
	{
		check_syntax_errors(tokens, &has_syntax_error);
		tree[i] = collect_redirection(tokens, has_syntax_error);
		if (tree[i].type == NODE_INVALID)
			*exit_status = 1;
		parse_branch(tokens, &tree[i]);
		i++;
	}
	if (has_syntax_error != false)
		return (*exit_status = 2, lst_memory(tree, NULL, FREE), NULL);
	return (tree);
}
