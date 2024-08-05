/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:46:26 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 17:17:58 by anarama          ###   ########.fr       */
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
		if (tokens[i].token_type == TOKEN_AND || tokens[i].token_type == TOKEN_OR || tokens[i].token_type == TOKEN_PIPE)
			tree_count++;
		i++;
	}
	return (tree_count);
}

void	print_branch(t_ast *g_tree)
{
	uint32_t	i;

	i = 0;
	while (g_tree[i].type != NODE_END)
	{
		printf("Branch %d\n", i);
		printf("Type: %d\n", g_tree[i].type);
		printf("Connection type: %d\n", g_tree[i].connection_type);
		printf("Has redir in: %d\n", g_tree[i].has_redir_in);
		printf("Has redir out: %d\n", g_tree[i].has_redir_out);
		printf("FD in: %d\n", g_tree[i].fd_in);
		printf("FD out: %d\n", g_tree[i].fd_out);
		printf("Args:\n");
		for (int j = 0; g_tree[i].args[j]; j++)
			printf("%s\n", g_tree[i].args[j]);
		i++;
	}
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

static void	parse_branch(t_token *tokens, t_ast *branch)
{
	char	**args;
	int		capacity;
	int		count;
	uint32_t	i;

	count = 0;
	capacity = INITIAL_TOKEN_CAPACITY;
	args = ft_calloc(capacity + 1, sizeof(char *));
	if (!args)
	{
		perror("calloc in parse tokens");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	i = 0;
	while (tokens[i].token_type != TOKEN_EOL \
			&& tokens[i].token_type != TOKEN_AND \
			&& tokens[i].token_type != TOKEN_OR \
			&& tokens[i].token_type != TOKEN_PIPE)
	{
		if (tokens[i].token_type != TOKEN_DONE && tokens[i].token_type != TOKEN_ENV && tokens[i].token_type != TOKEN_DOUBLE_QUOTE)
		{
			fill_args(&args, count, tokens[i].token_value, &capacity);
			tokens[i].token_type = TOKEN_DONE;
			count++;
		}
		i++;
	}
	if (tokens[i].token_type != TOKEN_EOL)
	{
		branch->connection_type = (t_tree_connection_type)tokens[i].token_type;
		tokens[i].token_type = TOKEN_DONE;
		tokens[i].token_value = NULL;
	}
	branch->args = args;
}

static t_ast	collect_redirection(t_token *token)
{
	uint32_t	i;
	t_ast		branch;
	
	i = 0;
	branch = (t_ast){0};
	branch.fd_in = STDIN_FILENO;
	branch.fd_out = STDOUT_FILENO;
	while (token[i].token_type != TOKEN_EOL \
			&& token[i].token_type != TOKEN_AND \
			&& token[i].token_type != TOKEN_OR \
			&& token[i].token_type != TOKEN_PIPE)

	{
		if (token[i].token_type != TOKEN_DONE 
			&& (token[i].token_type == TOKEN_REDIRECT_IN \
			|| token[i].token_type == TOKEN_REDIRECT_OUT \
			|| token[i].token_type == TOKEN_REDIRECT_APPEND)
		   )		{
			if  (token[i + 1].token_type == TOKEN_WORD || token[i + 1].token_type == TOKEN_ENV)
			{
				if (token[i].token_type == TOKEN_REDIRECT_IN)
				{
					if (branch.has_redir_in == true)
						ft_close(branch.fd_in, "fd_in in collect_redirection");
					ft_open(&branch.fd_in, token[i + 1].token_value, O_RDONLY, 0644);
					branch.has_redir_in = true;
				}
				else if (token[i].token_type == TOKEN_REDIRECT_OUT)
				{
					if (branch.has_redir_out == true)
						ft_close(branch.fd_out, "fd_out in collect_redirection");
					ft_open(&branch.fd_out, token[i + 1].token_value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					branch.has_redir_out = true;
				}
				else if (token[i].token_type == TOKEN_REDIRECT_APPEND)
				{
					if (branch.has_redir_out == true)
						ft_close(branch.fd_in, "fd_append in collect_redirection");
					ft_open(&branch.fd_in, token[i + 1].token_value, O_WRONLY | O_CREAT | O_APPEND, 0644);
					branch.has_redir_out = true;
				}
				token[i].token_type = TOKEN_DONE;
				token[i + 1].token_type = TOKEN_DONE;
				i++;
			}
			else
			{
				branch.type = NODE_INVALID;
				return (branch);
			}
		}
		i++;
	}
	return (branch);
}

t_ast	*parse_tokens(t_token *tokens)
{
	t_ast	*tree;
	int		i;
	uint32_t	tree_count;

	if (!tokens)
		return (NULL);
	i = 0;
	tree_count = determine_trees(tokens);
	tree = ft_calloc(tree_count + 1, sizeof(t_ast));
	lst_memory(tree, tree_destroy, ADD, 0);
	tree[tree_count].type = NODE_END;
	while (tree[i].type != NODE_END)
	{
		tree[i] = collect_redirection(tokens);
		parse_branch(tokens, &tree[i]);
		if (tree[i].type == NODE_INVALID)
			return (perror("minishell"), ft_free(&tree), NULL);
		i++;
	}
	return (tree);
}
