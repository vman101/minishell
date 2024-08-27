/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:17:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/27 14:14:23 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	fd_out(t_ast *command, int *exit_status)
{
	if (command->has_redir_out && command->path_file_out)
	{
		if (command->fd_out > 0)
		{
			ft_dup2(command->fd_out, STDOUT_FILENO, "in hanlde_fds_child");
		}
		else
			return (*exit_status = 1, false);
	}
	return (*exit_status = 0, true);
}

bool	handle_fds_child_proccess(t_ast *command, int32_t *exit_status)
{
	if (command->has_redir_in && command->path_file_in != 0)
	{
		if (access(command->path_file_in, F_OK) == 0)
		{
			ft_open(&command->fd_in, command->path_file_in, O_RDONLY, 0644);
			if (command->is_heredoc == true)
			{
				command->is_heredoc = false;
				if (unlink(command->path_file_in))
					perror("unlink");
				ft_free(&command->path_file_in);
			}
			ft_dup2(command->fd_in, STDIN_FILENO, "in hanlde_fds_child");
		}
		else
			return (p_stderr(2, "minishell: %s: No such file or directory\n", \
				command->path_file_in), command->type = NODE_INVALID, \
					*exit_status = 1, false);
	}
	if (command->has_redir_out && command->path_file_out)
		return (fd_out(command, exit_status));
	return (*exit_status = 0, true);
}

void	handle_fds_parent_proccess(t_ast *command, int32_t *exit_status)
{
	if (command->has_redir_in)
	{
		if (command->fd_in == -1 && command->path_file_in == 0)
			*exit_status = 1;
		if (command->is_heredoc == true && command->path_file_in)
			ft_free(&command->path_file_in);
	}
	if (command->has_redir_out && command->path_file_out && command->fd_out > 0)
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
