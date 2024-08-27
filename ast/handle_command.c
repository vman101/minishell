/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 18:14:10 by anarama           #+#    #+#             */
/*   Updated: 2024/08/27 15:22:42 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	buildin_apply_pipe(t_ast *node, int32_t *exit_status)
{
	if (node->type == NODE_PIPE)
	{
		ft_dup2(node->pipefd[1], STDOUT_FILENO, "dup2 in buildin_execute");
		ft_close(node->pipefd[1], "close in buildin_execute");
	}
	if (node->has_redir_in || node->has_redir_out)
	{
		return (handle_fds_child_proccess(node, exit_status));
	}
	return (true);
}

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
		if (is_buildin(command) && should_execute)
			buildin_execute(command, (const char **)env, exit_status);
		else if (path && should_execute)
		{
			execve(path, command->args, (char **)env);
			perror("execve");
		}
		return (lst_memory(NULL, NULL, END), \
				close_fds(std), exit(*exit_status));
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
	if (ft_memcmp(node->args[0], "echo", ft_strlen(node->args[0]) + 1) == 0)
		return (ft_echo(node->args, e), 1);
	else if (ft_memcmp(node->args[0], "env", ft_strlen(node->args[0]) + 1) == 0)
		return (ft_env(environment, e), 1);
	else if (ft_memcmp(node->args[0], "cd", ft_strlen(node->args[0]) + 1) == 0)
		return (ft_cd(environment, \
					(const char **)node->args, e), 1);
	else if (ft_memcmp(node->args[0], "pwd", ft_strlen(node->args[0]) + 1) == 0)
		return (ft_pwd(environment, e), 1);
	else if (ft_memcmp(node->args[0], "unset", \
				ft_strlen(node->args[0]) + 1) == 0)
		return (ft_unset((char **)environment, \
					(const char **)node->args, e), 1);
	else if (ft_memcmp(node->args[0], "export", \
				ft_strlen(node->args[0]) + 1) == 0)
		ft_export((const char **)node->args, e);
	else if (ft_memcmp(node->args[0], "exit", \
			ft_strlen(node->args[0]) + 1) == 0)
		return (ft_exit(node, e), *e = 1);
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
	if (is_buildin(current) && current->type != NODE_PIPE \
			&& current->was_pipe == false)
	{
		if (buildin_apply_pipe(current, exit_status))
			buildin_execute(current, env, exit_status);
	}
	else
	{
		path = NULL;
		if (current->args && current->args[0] && current->args[0][0])
		{
			if (!is_buildin(current))
				path = find_absolute_path(\
						environment_variable_value_get("PATH", \
							env), current->args[0], exit_status);
		}
		execute_command(current, exit_status, std, path);
	}
}
