/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/19 18:18:14 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int	org_stdout;

	org_stdout = dup(STDOUT_FILENO);
	if (org_stdout == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
	if (command->connection_type == TREE_PIPE)
		handle_pipe_in_parent(command);
	if (command->has_redir_in || command->has_redir_out)
		handle_fds_parent_proccess(command, exit_status);
	waitpid(pid, exit_status, 0);
	if (WIFEXITED(*exit_status))
	{
		dup2(org_stdout, STDOUT_FILENO);
		ft_close(org_stdout, "in execute_parent");
		*exit_status = WEXITSTATUS(*exit_status);
	}
}

void	execute_command(t_ast *command, const char **env, int32_t *exit_status, char *path)
{
	pid_t	pid;

	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		if (command->connection_type == TREE_PIPE)
			handle_pipe_in_child(command);
		if (command->has_redir_in || command->has_redir_out)
			handle_fds_child_proccess(command);
		execve(path, command->args, (char **)env);
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
		ft_close(node->pipefd[1], "close in buildin_execute");
	}
	if (node->has_redir_in || node->has_redir_out)
	{
		handle_fds_child_proccess(node);
	}
}

bool	buildin_execute(t_ast *node, const char **environment, int *exit_status)
{
	if (!node->args || !*node->args)
		return (buildin_apply_pipe(node), false);
	if (ft_memcmp(node->args[0], "echo", ft_strlen("echo") + 1) == 0)
		return (buildin_apply_pipe(node), ft_echo(node->args, exit_status), 1);
	else if (ft_memcmp(node->args[0], "env", ft_strlen("env") + 1) == 0)
		return (buildin_apply_pipe(node), ft_env(environment, exit_status), 1);
	else if (ft_memcmp(node->args[0], "cd", ft_strlen("cd") + 1) == 0)
		return (buildin_apply_pipe(node), ft_cd(environment, \
					(const char **)node->args, exit_status), 1);
	else if (ft_memcmp(node->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
		return (ft_pwd(environment, exit_status), 1);
	else if (ft_memcmp(node->args[0], "unset", ft_strlen("unset") + 1) == 0)
		return (buildin_apply_pipe(node), ft_unset((char **)environment, \
					(const char **)node->args, exit_status), 1);
	else if (ft_memcmp(node->args[0], "export", ft_strlen("export") + 1) == 0)
	{
		buildin_apply_pipe(node);
		if (node->connection_type != TREE_PIPE)
			ft_export((const char **)node->args, exit_status);
		return (1);
	}
	else if (ft_memcmp(node->args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (ft_exit((const char **)node->args), *exit_status = 1);
	return (0);
}

void	clear_empty_args(char **args)
{
	uint32_t	i;

	i = 0;
	while (args[i] && args[i][0] == 0)
	{
		if (args[i + 1])
		{
			while (args[i])
			{
				args[i] = args[i + 1];
				i++;
			}
		}
		else
			i++;
	}
}

void	handle_command(t_ast *current, const char **env, int *exit_status)
{
	int32_t		stdout_org;
	char		*path_variable;
	char		*path;

	stdout_org = dup(STDOUT_FILENO);
	if (stdout_org == -1)
		return (perror("dup"));
	if (current->connection_type == TREE_PIPE)
		ft_pipe(current->pipefd, "in handle_command");
	path_variable = environment_variable_value_get("PATH", env);
	check_and_expand_wildcards(&current->args);
	clear_empty_args(current->args);
	if (!buildin_execute(current, env, exit_status))
	{
		path = find_absolute_path(path_variable, current->args[0]);
		if (!path)
			*exit_status = 127;
		else
			execute_command(current, env, exit_status, path);
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
	ft_close(stdout_org, "stdorg_in hanlde_command");
}
