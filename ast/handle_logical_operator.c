/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_logical_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:31:08 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 19:49:59 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast *skip_up_to_next_logical_operator(t_ast *ast)
{
	t_ast	*current;

	current = ast->right;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type == NODE_LOGICAL_OPERATOR)
		{
			current->is_done = 1;
			break ;
		}
		current->is_done = 1;
		current = current->right;
	}
	return (current);
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
