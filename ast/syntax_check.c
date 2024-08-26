/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:19:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/26 14:18:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_valid_redir_helper(t_token *token, int index, int *error_catched)
{
	if (token[index + 1].token_type == TOKEN_AND
		|| token[index + 1].token_type == TOKEN_OR)
	{
		print_error_logical_operator(token[index + 1].token_type);
		*error_catched = 1;
	}
	else if (token[index + 1].token_type == TOKEN_PIPE)
		return (print_error_pipe(), *error_catched = 1, (void)0);
	else if (token[index + 1].token_type == TOKEN_REDIRECT_IN \
			|| token[index + 1].token_type == TOKEN_REDIRECT_OUT \
			|| token[index + 1].token_type == TOKEN_REDIRECT_APPEND \
			|| token[index + 1].token_type == TOKEN_HEREDOC)
	{
		print_error_redir(token[index + 1].token_type);
		*error_catched = 1;
	}
}

void	check_valid_redir(t_token *token, int index, int *error_catched)
{
	if (token[index + 1].token_type == TOKEN_EOL \
		|| token[index + 1].token_type == TOKEN_NEWLINE \
		|| token[index + 1].token_type == 0 \
		|| (token[index + 1].token_type == TOKEN_WORD \
		&& token[index + 1].token_value == NULL))
	{
		ft_putendl_fd(\
				"minishell: syntax error near unexpected token `newline'", 2);
		*error_catched = 1;
	}
	check_valid_redir_helper(token, index, error_catched);
}

void	check_valid_pipe(t_token *token, int index, int *error_catched)
{
	if (index == 0 || token[index + 1].token_type == 0 \
		|| token[index + 1].token_type == TOKEN_EOL
		|| (token[index + 1].token_type == TOKEN_WORD \
		&& token[index + 1].token_value == NULL))
	{
		print_error_pipe();
		*error_catched = 1;
	}
	else if (token[index + 1].token_type == TOKEN_AND
		|| token[index + 1].token_type == TOKEN_OR)
	{
		print_error_logical_operator(token[index + 1].token_type);
		*error_catched = 1;
	}
	else if (token[index + 1].token_type == TOKEN_PIPE)
	{
		print_error_pipe();
		*error_catched = 1;
	}
}

void	check_valid_logical_operator(	t_token *token, \
										int index, \
										int *error_catched)
{
	if (index == 0 || token[index + 1].token_type == TOKEN_EOL \
			|| token[index + 1].token_type == TOKEN_NEWLINE \
			|| token[index + 1].token_type == 0)
	{
		print_error_logical_operator(token[index].token_type);
		*error_catched = 1;
	}
	else if (token[index + 1].token_type == TOKEN_AND
		|| token[index + 1].token_type == TOKEN_OR)
	{
		print_error_logical_operator(token[index + 1].token_type);
		*error_catched = 1;
	}
	else if (token[index + 1].token_type == TOKEN_PIPE)
	{
		print_error_pipe();
		*error_catched = 1;
	}
}

void	check_valid_heredoc(t_token *token, int index, int *error_catched)
{
	if (token[index + 1].token_value == 0 \
	|| *token[index + 1].token_value == 0 || token[index + 1].token_type == 0)
	{
		*error_catched = 2;
		if (token[index + 1].token_type == TOKEN_AND \
				|| token[index + 1].token_type == TOKEN_OR)
			print_error_logical_operator(token[index].token_type);
		else if (token[index + 1].token_type == TOKEN_PIPE)
			print_error_pipe();
		else if (token[index + 1].token_type == TOKEN_REDIRECT_IN \
				|| token[index + 1].token_type == TOKEN_REDIRECT_OUT \
				|| token[index + 1].token_type == TOKEN_REDIRECT_APPEND \
				|| token[index + 1].token_type == TOKEN_HEREDOC)
			print_error_redir(token[index + 1].token_type);
		else
			ft_putendl_fd(\
				"minishell: syntax error near unexpected token `newline'", 2);
	}
}
