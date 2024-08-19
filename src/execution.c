/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:54:19 by victor            #+#    #+#             */
/*   Updated: 2024/08/19 23:43:17 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	command_execute(	const char *command_path,
							const char **argv,
							const char **env)
{
	pid_t	child_proccess;

	ft_fork(&child_proccess, command_path);
	if (child_proccess == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_proccess, NULL, 0);
	}
}

static void	execution_loop_helper(	t_ast *tree, \
									uint32_t *i, \
									int32_t std[2], \
									int32_t *exit_status)
{
	if (tree[*i].connection_type != TREE_PIPE)
		restore_fd(std[0], std[1]);
	if ((tree[*i].connection_type == TREE_LOGICAL_OR \
		&& *exit_status == 0))
		i++;
	else if ((tree[*i].connection_type == TREE_LOGICAL_AND \
		&& *exit_status != 0))
		i++;
}

void	execution_loop(	t_ast *tree, \
						const char **env, \
						int *exit_status)
{
	bool		error_found;

	error_found = false;
	if (tree->connection_type != TREE_INVALID)
	{
		if (tree->has_redir_in \
			&& tree->path_file_in == 0 \
			&& tree->fd_in == -1)
		{
			return (*exit_status = 1, (void)0);
		}
		evaluate_input(&tree->args, exit_status, &error_found);
		if (tree->args && tree->args[0])
			environment_variable_value_change(env, "_", tree->args[0]);
		if (*exit_status == -1 || error_found == true)
			return (*exit_status = 2, (void)0);
		handle_command(tree, env, exit_status);
	}
}

void	execute_commands(t_ast *tree, const char **env, int *exit_status)
{
	int32_t		stdin_org;
	int32_t		stdout_org;
	uint32_t	i;

	i = 0;
	stdin_org = dup(STDIN_FILENO);
	stdout_org = dup(STDOUT_FILENO);
	if (stdout_org == -1 || stdin_org == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
	while (tree[i].type != NODE_END)
	{
		execution_loop(&tree[i], env, exit_status);
		execution_loop_helper(tree, &i, \
								(int [2]){stdin_org, stdout_org}, \
								exit_status);
		i++;
	}
	ft_close(stdin_org, "stdin in restore_fd");
	ft_close(stdout_org, "stdout in restore_fd");
}
