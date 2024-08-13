/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/10 22:28:51 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>

void	handle_redir_in(t_ast *branch, \
						t_token *token, \
						t_token *token_next)
{
	int32_t	i;
	char	**temp;

	i = 0;
	if (token->token_type == TOKEN_REDIRECT_IN)
	{
		temp = (char **)(char *[]){token_next->token_value, NULL};
		evaluate_input(&temp, &i, 0);
		token_next->token_value = temp[0];
		if (branch->has_redir_in == true)
			ft_close(branch->fd_in, "fd_in in collect_redirection");
		ft_open(&branch->fd_in, token_next->token_value, O_RDONLY, 0644);
		if (branch->fd_in == -1)
			branch->connection_type = TREE_INVALID;
		branch->has_redir_in = true;
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}

void	handle_redir_out(	t_ast *branch, \
							t_token *token, \
							t_token *token_next)
{
	int32_t	i;
	char	**temp;

	i = 0;
	if (token->token_type == TOKEN_REDIRECT_OUT)
	{
		temp = (char **)(char *[]){token_next->token_value, NULL};
		evaluate_input(&temp, &i, 0);
		token_next->token_value = temp[0];
		if (branch->has_redir_out == true)
			ft_close(branch->fd_out, "fd_in in collect_redirection");
		ft_open(&branch->fd_out, token_next->token_value, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (branch->fd_out == -1)
			branch->connection_type = TREE_INVALID;
		branch->has_redir_out = true;
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}

void	handle_redir_append(t_ast *branch, \
							t_token *token, \
							t_token *token_next)
{
	int32_t	i;
	char	**temp;

	i = 0;
	if (token->token_type == TOKEN_REDIRECT_APPEND)
	{
		temp = (char **)(char *[]){token_next->token_value, NULL};
		evaluate_input(&temp, &i, 0);
		token_next->token_value = temp[0];
		if (branch->has_redir_out == true)
			ft_close(branch->fd_out, "fd_append in collect_redirection");
		ft_open(&branch->fd_out, token_next->token_value, \
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (branch->fd_out == -1)
			branch->connection_type = TREE_INVALID;
		branch->has_redir_out = true;
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}

void	handle_heredoc(t_token *tokens, int32_t pipefd)
{
	uint32_t	i;
	char		*value;
	uint32_t	value_length;
	uint32_t	token_length;

	i = 1;
	value = tokens[0].token_value;
	value_length = ft_strlen(value);
	while (tokens[i].token_type != TOKEN_EOL)
	{
		token_length = ft_strlen(tokens[i].token_value);
		if (token_length >= value_length)
			if (ft_strncmp(value, tokens[i].token_value, token_length) == 0)
				break ;
		ft_putendl_fd(tokens[i].token_value, pipefd);
		tokens[i].token_type = TOKEN_DONE;
		i++;
	}
	tokens[i].token_type = TOKEN_DONE;
}

bool	heredoc_has_been_done(t_token *token)
{
	uint32_t	i;
	char		*value;
	uint32_t	value_length;
	uint32_t	token_length;

	i = 1;
	value = token->token_value;
	value_length = ft_strlen(value);
	while (!is_delimiter_token(&token[i]))
	{
		token_length = ft_strlen(token[i].token_value);
		if (token_length >= value_length)
			if (ft_strncmp(value, token[i].token_value, token_length) == 0)
				return (true);
		i++;
	}
	return (false);
}

void	handle_redir_heredoc(	t_ast *branch, \
								t_token *token, \
								t_token *token_next, \
								const char **environment)
{
	if (token->token_type == TOKEN_HEREDOC)
	{
		ft_pipe(branch->pipefd, "here_doc");
		if (!heredoc_has_been_done(token_next))
		{
			token_heredoc_get(token, token_next->token_value, environment);
			ft_putstr_fd(token->token_value, branch->pipefd[1]);
			token->token_type = TOKEN_DONE;
			token_next->token_type = TOKEN_DONE;
		}
		else
			handle_heredoc(token_next, branch->pipefd[1]);
		ft_close(branch->pipefd[1], "pipe[1] in heredoc");
		branch->fd_in = branch->pipefd[0];
		branch->has_redir_in = true;
	}
}