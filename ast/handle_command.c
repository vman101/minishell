/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 16:42:32 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_ast *command, const char **env)
{
	int		status;
	pid_t	pid;

	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		handle_fds_child_proccess(command);
		if (command->path)
		{
			if (command->fd_in != -1 && command->fd_out != -1)
				execve(command->path, command->args, (char **)env);
			perror("execve");
		}
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		handle_fds_parent_proccess(command);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			return (WEXITSTATUS(status));
		}
	}
	return (-1);
}

void	handle_command(t_ast *current, const char *path_variable,
					const char **env, int *exit_status)
{
	int		original_stdin;
	int		original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (!buildin_execute(current->args[0],
			(const char **)current->args))
	{
		current->path = find_absolute_path(path_variable,
				current->args[0]);
		*exit_status = execute_command(current, env);
		if (current->file)
			restore_fd(original_stdin, original_stdout);
	}
}
