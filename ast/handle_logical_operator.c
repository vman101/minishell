/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_logical_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:31:08 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 17:30:35 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_up_to_next_logical_operator(t_ast *ast)
{
	t_ast *current = ast->right;
	current->is_done = 1;
	while (current->right)
	{
		if (current->right->type == NODE_LOGICAL_OPERATOR)
			break ;
		current->right->is_done = 1;
		current->right = current->right->right;
	}
}

void	handle_logical_operator(t_ast *logical_node, int exit_status)
{
	if (logical_node->token_type == TOKEN_AND)
	{
		if (exit_status == 1)
		{
			skip_up_to_next_logical_operator(logical_node);
		}
	}
	else if (logical_node->token_type == TOKEN_OR)
	{
		if (exit_status == 0)
		{
			skip_up_to_next_logical_operator(logical_node);
		}
	}
}
