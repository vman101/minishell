/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:19:12 by anarama           #+#    #+#             */
/*   Updated: 2024/08/09 14:01:11 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_logical_operator(t_token_type token_type)
{
	if (token_type == TOKEN_AND)
		printf("minishell: syntax error near unexpected token `&&'\n");
	if (token_type == TOKEN_OR)
		printf("minishell: syntax error near unexpected token `||'\n");
}

void	print_error_pipe()
{
	printf("minishell: syntax error near unexpected token `|'\n");
}

void	print_error_redir(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_APPEND)
		printf("minishell: syntax error near unexpected token `>>'\n");
	else if (token_type == TOKEN_REDIRECT_IN)
		printf("minishell: syntax error near unexpected token `<'\n");
	else if (token_type == TOKEN_REDIRECT_OUT)
		printf("minishell: syntax error near unexpected token `>'\n");
	else if (token_type == TOKEN_HEREDOC)
		printf("minishell: syntax error near unexpected token `<<'\n");
}

void	check_valid_redir(t_token *token, int index, int *error_catched)
{
	if (token[index + 1].token_type == TOKEN_EOL
		|| (token[index + 1].token_type == TOKEN_WORD && token[index + 1].token_value == NULL))
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		*error_catched = 1;
	}
	if (token[index + 1].token_type == TOKEN_AND
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
	else if (token[index + 1].token_type == TOKEN_REDIRECT_IN \
			|| token[index + 1].token_type == TOKEN_REDIRECT_OUT \
			|| token[index + 1].token_type == TOKEN_REDIRECT_APPEND \
			|| token[index + 1].token_type == TOKEN_HEREDOC)
	{
		print_error_redir(token[index + 1].token_type);
		*error_catched = 1;
	}
}

void	check_valid_pipe(t_token *token, int index, int *error_catched)
{
	if (index == 0 || token[index + 1].token_type == TOKEN_EOL
	|| (token[index + 1].token_type == TOKEN_WORD && token[index + 1].token_value == NULL))
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

void	check_valid_logical_operator(t_token *token, int index, int *error_catched)
{
	if (index == 0 || token[index + 1].token_type == TOKEN_EOL)
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