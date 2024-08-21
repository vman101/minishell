/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:17:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/21 12:47:39 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_fds_child_proccess(t_ast *command)
{
	if (command->has_redir_in)
	{
		if (command->path_file_in != 0)
			ft_open(&command->fd_in, command->path_file_in, O_RDONLY, 0644);
		dup2(command->fd_in, STDIN_FILENO);
		ft_close(command->fd_in, "in child");
		command->fd_in = -1;
	}
	if (command->has_redir_out)
	{
		dup2(command->fd_out, STDOUT_FILENO);
		ft_close(command->fd_out, "in child");
		command->fd_out = -1;
	}
}

void	handle_fds_parent_proccess(t_ast *command, int32_t *exit_status)
{
	if (command->has_redir_in)
	{
		if (command->fd_in == -1 && command->path_file_in == 0)
			*exit_status = 1;
	}
	if (command->has_redir_out)
	{
		ft_close(command->fd_out, "in hanlde_fds_parents");
		command->fd_out = -1;
	}
}

void	handle_pipe_in_child(t_ast *command)
{
	ft_dup2(command->pipefd[1], STDOUT_FILENO, "dup2 in pipe_child");
	ft_close(command->pipefd[0], "close in pipe_child");
	ft_close(command->pipefd[1], "close in pipe_child");
}

void	handle_pipe_in_parent(t_ast *command)
{
	ft_dup2(command->pipefd[0], STDIN_FILENO, "dup2 in pipe_parent");
	ft_close(command->pipefd[1], "close in pipe_parent");
	ft_close(command->pipefd[0], "close in pipe_parent");
}

void	buildin_apply_pipe(t_ast *node)
{
	if (node->type == NODE_PIPE)
	{
		ft_dup2(node->pipefd[1], STDOUT_FILENO, "dup2 in buildin_execute");
		ft_close(node->pipefd[1], "close in buildin_execute");
	}
	if (node->has_redir_in || node->has_redir_out)
	{
		handle_fds_child_proccess(node);
	}
}
