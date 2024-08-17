/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:46:26 by anarama           #+#    #+#             */
/*   Updated: 2024/08/17 18:08:02 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static uint32_t	determine_trees(t_token *tokens)
{
	uint32_t	tree_count;
	uint32_t	i;

	i = 0;
	tree_count = 1;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		if (tokens[i].token_type == TOKEN_AND \
			|| tokens[i].token_type == TOKEN_OR \
			|| tokens[i].token_type == TOKEN_PIPE \
			|| tokens[i].token_type == TOKEN_NEWLINE)
			tree_count++;
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
	while (tree[i].type != NODE_END)
	{
		if (tree[i].has_redir_in == true)
			ft_close(tree[i].fd_in, "fd_in in tree_destroy");
		if (tree[i].has_redir_out == true)
			ft_close(tree[i].fd_out, "fd_out in tree_destroy");
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
			|| token->token_type == TOKEN_PIPE);
}

static void	parse_branch(t_token *tokens, t_ast *branch)
{
	int			capacity;
	int			count;
	uint32_t	i;

	count = 0;
	capacity = INITIAL_TOKEN_CAPACITY;
	branch->args = ft_calloc(capacity + 1, sizeof(char *));
	if (!branch->args)
	{
		perror("calloc in parse tokens");
		lst_memory(NULL, NULL, CLEAN);
	}
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
	branch->connection_type = (t_tree_connection_type)tokens[i].token_type;
	tokens[i].token_type = TOKEN_DONE;
}

static t_ast	collect_redirection(t_token *token, \
									const char **env, \
									bool has_syntax_error)
{
	uint32_t	i;
	t_ast		branch;

	i = 0;
	branch = (t_ast){0};
	branch.fd_in = STDIN_FILENO;
	branch.fd_out = STDOUT_FILENO;
	while (!is_delimiter_token(&token[i]) && branch.connection_type != TREE_INVALID)
	{
		if (token[i + 1].token_type == TOKEN_WORD)
		{
			if (!has_syntax_error)
			{
				handle_redir_in(&branch, &token[i], &token[i + 1]);
				handle_redir_out(&branch, &token[i], &token[i + 1]);
				handle_redir_append(&branch, &token[i], &token[i + 1]);
			}
			handle_redir_heredoc(&branch, &token[i], &token[i + 1]);
		}
		i++;
	}
	return (branch);
}

int	check_syntax_errors(t_token *token)
{
	int	error_catched;
	int	i;

	error_catched = 0;
	i = 0;
	while (!is_delimiter_token(&token[i]) || token[i].token_type == TOKEN_PIPE)
	{
		if (token[i].token_type == TOKEN_REDIRECT_IN \
			|| token[i].token_type == TOKEN_REDIRECT_OUT \
			|| token[i].token_type == TOKEN_REDIRECT_APPEND \
			|| token[i].token_type == TOKEN_HEREDOC)
			check_valid_redir(token, i, &error_catched);
		else if (token[i].token_type == TOKEN_PIPE)
			check_valid_pipe(token, i, &error_catched);
		else if (token[i].token_type == TOKEN_AND \
				|| token[i].token_type == TOKEN_OR)
			check_valid_logical_operator(token, i, &error_catched);
		if (error_catched)
			return (0);
		i++;
	}
	return (1);
}

t_ast	*parse_tokens(	t_token *tokens, \
						const char **environment, \
						int32_t *exit_status)
{
	t_ast		*tree;
	int			i;
	uint32_t	tree_count;
	bool		has_syntax_error;

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
		if (!check_syntax_errors(tokens))
			has_syntax_error = true;
		tree[i] = collect_redirection(tokens, environment, has_syntax_error);
		if (tree[i].connection_type == TREE_INVALID)
			*exit_status = 1;
		parse_branch(tokens, &tree[i]);
		i++;
	}
	if (has_syntax_error == true)
		return (*exit_status = 2, lst_memory(tree, NULL, FREE), NULL);
	return (tree);
}
