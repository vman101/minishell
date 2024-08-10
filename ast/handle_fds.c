/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:17:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/10 22:26:00 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_fds_child_proccess(t_ast *command)
{
	if (command->has_redir_in)
	{
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

void	handle_fds_parent_proccess(t_ast *command)
{
	if (command->has_redir_in)
	{
		ft_close(command->fd_in, "in hanlde_fds_parents");
		command->fd_in = -1;
	}
	if (command->has_redir_out)
	{
		ft_close(command->fd_out, "in hanlde_fds_parents");
		command->fd_out = -1;
	}
}
