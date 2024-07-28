/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_logical_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:31:08 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 13:50:39 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_up_to_next_logical_operator(t_ast **ast)
{
	(*ast)->is_done = 1;
	while ((*ast)->right)
	{
		if ((*ast)->right->type == NODE_LOGICAL_OPERATOR)
			break ;
		(*ast)->right->is_done = 1;
		(*ast)->right = (*ast)->right->right;
	}
}

void	handle_logical_operator(t_ast **logical_node, int exit_status)
{
	if ((*logical_node)->token_type == TOKEN_AND)
	{
		if (exit_status == 1)
		{
			skip_up_to_next_logical_operator(logical_node);
		}
	}
	else if ((*logical_node)->token_type == TOKEN_OR)
	{
		if (exit_status == 0)
		{
			skip_up_to_next_logical_operator(logical_node);
		}
	}
}
