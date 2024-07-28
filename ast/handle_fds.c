/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:17:10 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 22:30:19 by andrejarama      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_fd_into_file(t_ast *command)
{
	if (command->fd_file_in)
	{
		dup2(command->fd_file_in, STDIN_FILENO);
		close(command->fd_file_in);
	}
	if (command->fd_file_out)
	{
		dup2(command->fd_file_out, STDOUT_FILENO);
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
	if (command->file)
	{
		redirect_fd_into_file(command);
	}
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
 	if (command->file)
	{
		redirect_fd_into_file(command);
	}
}
