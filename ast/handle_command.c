/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/09 13:18:13 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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

void	execute_parent(t_ast *command, int32_t *exit_status, pid_t pid)
{
	int		stdout_fd;

	stdout_fd = dup(STDOUT_FILENO);
	if (stdout_fd == -1)
		lst_memory(NULL, NULL, CLEAN);
	if (command->connection_type == TREE_PIPE)
		handle_pipe_in_parent(command);
	if (command->has_redir_in || command->has_redir_out)
		handle_fds_parent_proccess(command);
	waitpid(pid, exit_status, 0);
	if (WIFEXITED(*exit_status))
	{
		dup2(stdout_fd, STDOUT_FILENO);
		ft_close(stdout_fd, "in execute command parent");
		*exit_status = WEXITSTATUS(*exit_status);
	}
}

void	execute_command(t_ast *command, const char **env, int32_t *exit_status)
{
	pid_t	pid;

	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		if (command->connection_type == TREE_PIPE)
			handle_pipe_in_child(command);
		if (command->has_redir_in || command->has_redir_out)
			handle_fds_child_proccess(command);
		execve(command->path, command->args, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
		execute_parent(command, exit_status, pid);
}

void	buildin_apply_pipe(t_ast *node)
{
	if (node->connection_type == TREE_PIPE)
	{
		ft_dup2(node->pipefd[1], STDOUT_FILENO, "dup2 in buildin_execute");
		// ft_close(node->pipefd[0], "close in buildin_execute");
		ft_close(node->pipefd[1], "close in buildin_execute");
	}
	if (node->has_redir_in || node->has_redir_out)
	{
		handle_fds_child_proccess(node);
	}
}

bool	buildin_execute(t_ast *node, const char **environment, int *exit_status)
{
	bool	is_buildin;

	if (node->args[0] && !*node->args[0])
		return (false);
	if (ft_strncmp(node->args[0], "echo", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_echo(node->args, exit_status), 1);
	else if (ft_strncmp(node->args[0], "env", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_env(environment, exit_status), 1);
	else if (ft_strncmp(node->args[0], "cd", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_cd(environment, (const char **)node->args, exit_status), 1);
	else if (ft_strncmp(node->args[0], "unset", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_unset((char **)environment, (const char **)node->args, exit_status), 1);
	else if (ft_strncmp(node->args[0], "export", ft_strlen(node->args[0])) == 0)
		return (buildin_apply_pipe(node), ft_export((char ***)&environment, (const char **)node->args, exit_status), 1);
	else if (ft_strncmp(node->args[0], "exit", ft_strlen(node->args[0])) == 0)
		return (ft_exit((const char **)node->args), 1);
	return (0);
}

void	handle_command(t_ast *current, const char *path_variable,
					const char **env, int *exit_status)
{
	int32_t		stdout_org;

	stdout_org = dup(STDOUT_FILENO);
	if (stdout_org == -1)
		return (perror("dup"));
	if (current->connection_type == TREE_PIPE)
		ft_pipe(current->pipefd, "in handle_command");
	if (!buildin_execute(current, env, exit_status))
	{
		current->path = find_absolute_path(path_variable, current->args[0]);
		if (!current->path)
			*exit_status = 127;
		else
			execute_command(current, env, exit_status);
	}
	else
	{
		if (current->connection_type == TREE_PIPE)
		{
			ft_dup2(current->pipefd[0], STDIN_FILENO, "in hanlde_command");
			ft_close(current->pipefd[0], "in handle_command");
			ft_dup2(stdout_org, STDOUT_FILENO, "dup2 in handle_command");
		}
	}
	ft_close(stdout_org, "in handle_command");
}
