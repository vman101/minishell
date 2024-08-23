/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:38:50 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/23 14:40:12 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_logical_operator(t_token_type token_type)
{
	if (token_type == TOKEN_AND)
		ft_putendl_fd("minishell: syntax error near unexpected token `&&'", 2);
	if (token_type == TOKEN_OR)
		ft_putendl_fd("minishell: syntax error near unexpected token `||'", 2);
}

void	print_error_pipe(void)
{
	ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
}

void	print_error_redir(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_APPEND)
		ft_putendl_fd("minishell: syntax error near unexpected token `>>'", 2);
	else if (token_type == TOKEN_REDIRECT_IN)
		ft_putendl_fd("minishell: syntax error near unexpected token `<'", 2);
	else if (token_type == TOKEN_REDIRECT_OUT)
		ft_putendl_fd("minishell: syntax error near unexpected token `>'", 2);
	else if (token_type == TOKEN_HEREDOC)
		ft_putendl_fd("minishell: syntax error near unexpected token `<<'", 2);
}
