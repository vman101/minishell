/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/19 19:46:27 by victor           ###   ########.fr       */
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
			branch->connection_type = TREE_INVALID;
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
	char		*value;
	char		*tmp;
	char		*tmp2;
	char		character_store;
	uint32_t	value_length;
	uint32_t	token_length;

	value = tokens[0].token_value;
	tmp = value;
	while (*tmp && ft_isalnum(*tmp))
		tmp++;
	character_store = *tmp;
	*tmp = 0;
	value_length = ft_strlen(value);
	*tmp = character_store;
	tmp = ft_strchr(value, '\n');
	if (tmp)
	{
		*tmp++ = 0;
		while (tmp)
		{
			tmp2 = tmp;
			tmp = ft_strchr(tmp2, '\n');
			if (tmp)
				*tmp++ = 0;
			token_length = ft_strlen(tmp2);
			if (token_length >= value_length)
				if (ft_memcmp(value, tmp2, token_length) == 0)
					break ;
			ft_putendl_fd(tmp2, pipefd);
		}
	}
	tokens->token_type = TOKEN_DONE;
}

bool	heredoc_has_been_done(char *value)
{
	char	*tmp;
	char	*tmp2;
	char	character_store;

	tmp = value;
	while (*tmp && !ft_isspace(*tmp))
		tmp++;
	character_store = *tmp;
	if (ft_isspace(*tmp) && *tmp != '\n')
	{
		*tmp = 0;
		tmp = ft_strchr(tmp, '\n');
		tmp += (tmp != 0);
	}
	else
		*tmp = 0;
	while (tmp && *tmp)
	{
		tmp2 = tmp + 1;
		tmp = ft_strchr(tmp + 1, '\n');
		if (tmp)
			*tmp = 0;
		if (ft_memcmp(value, tmp2, ft_strlen(value)) == 0)
		{
			if (tmp)
				*tmp = '\n';
			*ft_strchr(value, 0) = character_store;
			return (true);
		}
		if (tmp)
			*tmp = '\n';
	}
	return (false);
}

void	handle_redir_heredoc(	t_ast *branch, \
								t_token *token)
{
	char	**environment;
	char	*previous_input;

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
