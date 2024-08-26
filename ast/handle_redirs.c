/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/26 18:21:56 by vvobis           ###   ########.fr       */
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
		if (branch->has_redir_in)
			ft_close(branch->fd_in, "in hanlde_redir_in");
		ft_open(&branch->fd_in, token_next->token_value, O_RDONLY, 0);
		if (branch->fd_in > 0)
		{
			branch->has_redir_in = true;
			branch->path_file_in = token_next->token_value;
		}
		else
			branch->type = NODE_INVALID;
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
		branch->flags = O_WRONLY | O_CREAT | O_TRUNC;
		if (branch->has_redir_out == true)
		{
			ft_open(&branch->fd_out, branch->path_file_out, \
					branch->flags, 0644);
			ft_close(branch->fd_out, "in branch redir_out");
		}
		branch->path_file_out = token_next->token_value;
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
		branch->flags = O_WRONLY | O_CREAT | O_APPEND;
		if (branch->has_redir_out == true)
		{
			ft_open(&branch->fd_out, branch->path_file_out, \
					branch->flags, 0644);
			ft_close(branch->fd_out, "in branch redir_out");
		}
		branch->path_file_out = token_next->token_value;
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
			token_heredoc_get(token, token[1].token_value);
			print_value(token->token_value, branch->fd_in);
		}
		ft_close(branch->fd_in, "fd_in in heredoc");
		branch->has_redir_in = true;
		token[0].token_type = TOKEN_DONE;
		token[1].token_type = TOKEN_DONE;
	}
}
