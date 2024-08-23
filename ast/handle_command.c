/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/23 17:03:31 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_parent(t_ast *command, int32_t *exit_status, pid_t pid)
{
	command->cpid = pid;
	if (command->type == NODE_PIPE)
		handle_pipe_in_parent(command);
	if (command->has_redir_in || command->has_redir_out)
		handle_fds_parent_proccess(command, exit_status);
}

void	execute_command(	t_ast *command, \
							int32_t *exit_status, \
							int std[2], \
							char *path)
{
	pid_t	pid;
	char	**env;
	bool	should_execute;

	should_execute = true;
	ft_fork(&pid, "execute command");
	if (pid == 0)
	{
		env = env_static(NULL);
		if (command->type == NODE_PIPE)
			handle_pipe_in_child(command);
		if (command->has_redir_in || command->has_redir_out)
			should_execute = handle_fds_child_proccess(command, exit_status);
		if (path && should_execute)
		{
			execve(path, command->args, (char **)env);
			perror("execve");
		}
		lst_memory(NULL, NULL, END);
		ft_close(std[0], "after execve");
		ft_close(std[1], "after execve");
		exit(*exit_status);
	}
	else
		execute_parent(command, exit_status, pid);
}

bool	buildin_execute(	t_ast *node, \
							const char **environment, \
							int *e)
{
	if (!node->args || !*node->args)
		return (false);
	if (ft_memcmp(node->args[0], "echo", ft_strlen("echo") + 1) == 0)
		return (buildin_apply_pipe(node, e), ft_echo(node->args, e), 1);
	else if (ft_memcmp(node->args[0], "env", ft_strlen("env") + 1) == 0)
		return (buildin_apply_pipe(node, e), ft_env(environment, e), 1);
	else if (ft_memcmp(node->args[0], "cd", ft_strlen("cd") + 1) == 0)
		return (buildin_apply_pipe(node, e), ft_cd(environment, \
					(const char **)node->args, e), 1);
	else if (ft_memcmp(node->args[0], "pwd", ft_strlen("pwd") + 1) == 0)
		return (ft_pwd(environment, e), 1);
	else if (ft_memcmp(node->args[0], "unset", ft_strlen("unset") + 1) == 0)
		return (buildin_apply_pipe(node, e), ft_unset((char **)environment, \
					(const char **)node->args, e), 1);
	else if (ft_memcmp(node->args[0], "export", ft_strlen("export") + 1) == 0)
	{
		buildin_apply_pipe(node, e);
		if (node->type != NODE_PIPE)
			ft_export((const char **)node->args, e);
		return (1);
	}
	else if (ft_memcmp(node->args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (buildin_apply_pipe(node, e), ft_exit(node, e), *e = 1);
	return (0);
}

void	handle_command(	t_ast *current, \
						const char **env, \
						int *exit_status, \
						int std[2])
{
	char		*path;

	if (current->type == NODE_PIPE)
		ft_pipe(current->pipefd, "in handle_command");
	check_and_expand_wildcards(&current->args);
	if (!buildin_execute(current, env, exit_status))
	{
		path = find_absolute_path(environment_variable_value_get("PATH", \
					env), current->args[0], exit_status);
		execute_command(current, exit_status, std, path);
	}
	else
		if (current->type == NODE_PIPE)
			return (\
			ft_dup2(current->pipefd[0], STDIN_FILENO, "in hanlde_command"), \
			ft_close(current->pipefd[0], "in handle_command"), \
			ft_dup2(std[1], STDOUT_FILENO, "dup2 in handle_command"));
}
