/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/21 12:49:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		branch->has_redir_in = true;
		if (access(token_next->token_value, F_OK) == 0)
			branch->path_file_in = token_next->token_value;
		else
		{
			branch->type = NODE_INVALID;
			branch->fd_in = -1;
			p_stderr(2, "minishell: %s: No such file or directory\n", token_next->token_value);
		}
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
			branch->type = NODE_INVALID;
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
			branch->type = NODE_INVALID;
		branch->has_redir_out = true;
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}
void	handle_redir_heredoc(	t_ast *branch, \
								t_token *token)
{
	char	**environment;

	environment = env_static(NULL);
	if (token->token_type == TOKEN_HEREDOC)
	{
		ft_pipe(branch->pipefd, "here_doc");
		if (isatty(0) && !heredoc_has_been_done(token->token_value))
		{
			token_heredoc_get(token, token->token_value, \
							(const char **)environment);
			ft_putstr_fd(token->token_value, branch->pipefd[1]);
		}
		else
			handle_heredoc(token, branch->pipefd[1]);
		ft_close(branch->pipefd[1], "pipe[1] in heredoc");
		branch->fd_in = branch->pipefd[0];
		branch->has_redir_in = true;
	}
}
