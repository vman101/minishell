/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/23 16:01:28 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

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
		branch->path_file_in = token_next->token_value;
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
								t_token *token, \
								uint8_t token_id)
{
	char	**environment;

	environment = env_static(NULL);
	if (token->token_type == TOKEN_HEREDOC)
	{
		branch->path_file_in = ft_strdup((char []){'.', '/', '.', 't', \
										'm', 'p', token_id + 33, 0});
		branch->is_heredoc = true;
		ft_open(&branch->fd_in, branch->path_file_in, O_CREAT | O_WRONLY, 0644);
		if (!heredoc_has_been_done(token, \
			token[1].token_value, branch->fd_in) && isatty(0))
		{
			token_heredoc_get(token, token[1].token_value, \
							(const char **)environment);
			print_value(token->token_value, branch->fd_in);
		}
		ft_close(branch->fd_in, "fd_in in heredoc");
		branch->has_redir_in = true;
		token[0].token_type = TOKEN_DONE;
		token[1].token_type = TOKEN_DONE;
	}
}
