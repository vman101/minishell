/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:17:10 by anarama           #+#    #+#             */
/*   Updated: 2024/07/27 19:06:44 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_fd_into_file(t_ast *command)
{
	if (command->fd_file_in)
	{
		dup2(command->fd_file_in, STDOUT_FILENO);
		close(command->fd_file_in);
	}
	if (command->fd_file_out)
	{
		dup2(command->fd_file_out, STDIN_FILENO);
		close(command->fd_file_out);
	}
}

void	handle_fds_child_proccess(t_ast *command)
{
	if (command->fd_in != 0)
	{
		dup2(command->fd_in, STDIN_FILENO);
		close(command->fd_in);
	}
	if (command->fd_out != 1)
	{
		dup2(command->fd_out, STDOUT_FILENO);
		close(command->fd_out);
	}
	// if (command->file)
	// {
	// 	redirect_fd_into_file(command);
	// }
}

void	handle_fds_parent_proccess(t_ast *command)
{
	if (command->fd_in != 0)
	{
		close(command->fd_in);
	}
	if (command->fd_out != 1)
	{
		close(command->fd_out);
	}
 	// if (command->file)
	// {
	// 	redirect_fd_into_file(command);
	// }
}
