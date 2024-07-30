/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 20:49:34 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/30 12:35:11 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	setup_pipe(t_ast *pipe_node, int pipe_fd[2],
		int *left_command_found, int *right_command_found)
{
	while (pipe_node->left)
	{
		if (pipe_node->left->token_type == NODE_COMMAND
			&& !pipe_node->left->is_done)
			break ;
		pipe_node->left = pipe_node->left->left;
	}
	if (pipe_node->left && pipe_node->left->token_type == NODE_COMMAND)
	{
		pipe_node->left->fd_out = pipe_fd[1];
		*left_command_found = 1;
	}
	while (pipe_node->right)
	{
		if (pipe_node->right->token_type == NODE_COMMAND
			&& !pipe_node->right->is_done)
			break ;
		pipe_node->right = pipe_node->right->right;
	}
	if (pipe_node->right && pipe_node->right->token_type == NODE_COMMAND)
	{
		pipe_node->right->fd_in = pipe_fd[0];
		*right_command_found = 1;
	}
}

void	restore_data(t_ast *pipe_node, t_ast *save_ptr_left,
			t_ast *save_ptr_right)
{
	pipe_node->left = save_ptr_left;
	pipe_node->right = save_ptr_right;
    pipe_node->is_done = 1;
}

void	handle_pipe(t_ast *pipe_node)
{
    int		pipe_fd[2];
	int		left_command_found;
	int		right_command_found;
	t_ast	*save_ptr_left;
	t_ast	*save_ptr_right;

	left_command_found = 0;
	right_command_found = 0;
	save_ptr_left = pipe_node->left;
	save_ptr_right = pipe_node->right;
	ft_pipe(pipe_fd, "pipe in handle pipe");
	setup_pipe(pipe_node, pipe_fd, &left_command_found, &right_command_found);
	if (!left_command_found || !right_command_found)
	{
		close_pipe(pipe_fd);
		pipe_node->error_found = 1;
	}
	restore_data(pipe_node, save_ptr_left, save_ptr_right);
}
