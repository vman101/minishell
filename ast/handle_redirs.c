/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/27 17:28:56 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

bool	handle_temp(t_token *token)
{
	char	*was_variable;
	int32_t	i;
	char	**temp;

	if (token->token_value && token->token_value[0] == '$')
		was_variable = token->token_value;
	else
		was_variable = NULL;
	temp = (char **)(char *[]){token->token_value, NULL};
	evaluate_input(&temp, &i, 0);
	token->token_value = temp[0];
	if (was_variable)
		if (temp[0][0] == 0)
			return (p_stderr(2, \
			"minishell: %s: ambiguos redirect\n", was_variable), false);
	return (true);
}

void	handle_redir_in(t_ast *branch, \
						t_token *token, \
						t_token *token_next)
{
	if (token->token_type == TOKEN_REDIRECT_IN)
	{
		if (branch->path_file_in == 0 \
			|| ft_strncmp(token_next->token_value, branch->path_file_in, \
			ft_strlen(branch->path_file_in)) != 0)
		{
			if (handle_temp(token_next))
			{
				if (branch->has_redir_in)
					ft_close(branch->fd_in, "in hanlde_redir_in");
				branch->path_file_in = token_next->token_value;
				ft_open(&branch->fd_in, branch->path_file_in, O_RDONLY, 0);
				branch->has_redir_in = true;
				if (branch->fd_in == -1)
					branch->type = NODE_INVALID;
			}
			else
				branch->type = NODE_INVALID;
		}
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}

void	handle_redir_out(	t_ast *branch, \
							t_token *token, \
							t_token *token_next)
{
	if (token->token_type == TOKEN_REDIRECT_OUT)
	{
		if (branch->path_file_out == 0 \
			|| ft_strncmp(token_next->token_value, branch->path_file_out, \
			ft_strlen(branch->path_file_out)) != 0)
		{
			if (handle_temp(token_next))
			{
				if (branch->has_redir_out == true)
					ft_close(branch->fd_out, "in branch redir_out");
				branch->path_file_out = token_next->token_value;
				ft_open(&branch->fd_out, branch->path_file_out, \
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
				branch->has_redir_out = true;
				if (branch->fd_out == -1)
					branch->type = NODE_INVALID;
			}
			else
				branch->type = NODE_INVALID;
		}
		token->token_type = TOKEN_DONE;
		token_next->token_type = TOKEN_DONE;
	}
}

void	handle_redir_append(t_ast *branch, \
							t_token *token, \
							t_token *token_next)
{
	if (token->token_type == TOKEN_REDIRECT_APPEND)
	{
		if (branch->path_file_out == 0 \
			|| ft_strncmp(token_next->token_value, branch->path_file_out, \
			ft_strlen(branch->path_file_out)) != 0)
		{
			if (handle_temp(token_next))
			{
				branch->path_file_out = token_next->token_value;
				if (branch->has_redir_out == true)
					ft_close(branch->fd_out, "in branch redir_out");
				ft_open(&branch->fd_out, branch->path_file_out, \
						O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (branch->fd_out == -1)
					branch->type = NODE_INVALID;
				branch->has_redir_out = true;
			}
			else
				branch->type = NODE_INVALID;
		}
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
		remove_quotes(token[1].token_value);
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
