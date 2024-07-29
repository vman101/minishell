/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:49:18 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 14:03:32 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_valid_redir(t_ast *redir_node, int *error_catched)
{
	if (!redir_node->file)
	{
		if (!redir_node->right)
			printf("minishell: syntax error near unexpected token 'newline'\n");
		else
		{
			if (redir_node->right->type == NODE_LOGICAL_OPERATOR
				|| redir_node->right->type == NODE_PIPE)
			{
				if (redir_node->right->token_type == TOKEN_AND)
					printf("minishell: syntax error near unexpected token `&&'\n");
				if (redir_node->right->token_type == TOKEN_OR)
					printf("minishell: syntax error near unexpected token `||'\n");
				if (redir_node->right->type == NODE_PIPE)
					printf("minishell: syntax error near unexpected token `|'\n");
				*error_catched = 1;
			}
			else if (redir_node->right->type == NODE_REDIRECTION)
			{
				if (redir_node->right->token_type == TOKEN_REDIRECT_APPEND)
					printf("minishell: syntax error near unexpected token `>>'\n");
				else if (redir_node->right->token_type == TOKEN_REDIRECT_IN)
					printf("minishell: syntax error near unexpected token `>'\n");
				else if (redir_node->right->token_type == TOKEN_REDIRECT_OUT)
					printf("minishell: syntax error near unexpected token `<'\n");
			}
		}
		*error_catched = 1;
	}
}

void	check_valid_pipe(t_ast *pipe_node, int *error_catched)
{
	if (!pipe_node->left || !pipe_node->right)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		*error_catched = 1;
	}
	else if (pipe_node->right->type == NODE_LOGICAL_OPERATOR
		|| pipe_node->right->type == NODE_PIPE)
	{
		if (pipe_node->right->token_type == TOKEN_AND)
			printf("minishell: syntax error near unexpected token `&&'\n");
		if (pipe_node->right->token_type == TOKEN_OR)
			printf("minishell: syntax error near unexpected token `||'\n");
		if (pipe_node->right->type == NODE_PIPE)
			printf("minishell: syntax error near unexpected token `|'\n");
		*error_catched = 1;
	}
}

void	check_valid_logical_operator(t_ast *logical_node, int *error_catched)
{
	if (!logical_node->left || !logical_node->right)
	{
		if (logical_node->token_type == TOKEN_AND)
			printf("minishell: syntax error near unexpected token `&&'\n");
		if (logical_node->token_type == TOKEN_OR)
			printf("minishell: syntax error near unexpected token `||'\n");
		*error_catched = 1;
	}
	else if (logical_node->right->type == NODE_LOGICAL_OPERATOR
		|| logical_node->right->type == NODE_PIPE)
	{
		if (logical_node->right->token_type == TOKEN_AND)
			printf("minishell: syntax error near unexpected token `&&'\n");
		if (logical_node->right->token_type == TOKEN_OR)
			printf("minishell: syntax error near unexpected token `||'\n");
		if (logical_node->right->type == NODE_PIPE)
			printf("minishell: syntax error near unexpected token `|'\n");
		*error_catched = 1;
	}
}
