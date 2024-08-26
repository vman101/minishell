/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 22:54:19 by victor            #+#    #+#             */
/*   Updated: 2024/08/26 15:34:12 by vvobis           ###   ########.fr       */
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

void	wait_pids(t_ast *tree, uint pid_count, \
					pid_t pid_final, int32_t *exit_status)
{
	uint	i;
	int		ret_test;
	pid_t	wait_pid_ret;

	i = 0;
	while (i <= pid_count)
	{
		if (tree[i].cpid != 0)
		{
			wait_pid_ret = wait(&ret_test);
			if (wait_pid_ret == -1 && g_signal_flag == 3)
			{
				ft_putstr_fd("Quit (core dumped)\n", 1);
				continue ;
			}
			else if (wait_pid_ret == -1 && g_signal_flag == 1)
				continue ;
			else if (wait_pid_ret == -1)
				return (perror("wait"), lst_memory(NULL, NULL, CLEAN));
			if (wait_pid_ret == pid_final)
				*exit_status = WEXITSTATUS(ret_test);
			tree[i].cpid = 0;
		}
		i++;
	}
}

static void	execution_loop_helper(	t_ast *tree, \
									uint32_t *i, \
									int32_t std[2], \
									int32_t *exit_status)
{
	if (tree[*i].type != NODE_PIPE)
	{
		restore_fd(std);
		if (tree[*i].cpid != 0)
			wait_pids(tree, *i, tree[*i].cpid, exit_status);
	}
	else
	{
		ft_dup2(std[1], STDOUT_FILENO, \
				"in execution_loop execution_loop_helper");
	}
	if ((tree[*i].type == NODE_LOGICAL_OR \
		&& *exit_status == 0))
		(*i)++;
	else if ((tree[*i].type == NODE_LOGICAL_AND \
		&& *exit_status != 0))
		(*i)++;
}

void	execution_loop(	t_ast *tree, \
						const char **env, \
						int *exit_status, \
						int std[2])
{
	bool		error_found;

	error_found = false;
	if (tree->type != NODE_INVALID)
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
		handle_command(tree, env, exit_status, std);
	}
}

void	execute_commands(	t_ast *tree, const char **env, \
							int *exit_status, int std[2])

{
	uint32_t	i;
	bool		was_pipe;

	i = 0;
	if (std[0] == -1 || std[1] == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
	was_pipe = false;
	while (tree[i].type != NODE_END)
	{
		if (was_pipe == true)
		{
			tree[i].was_pipe = true;
			was_pipe = false;
		}
		execution_loop(&tree[i], env, exit_status, std);
		execution_loop_helper(tree, &i, std, exit_status);
		if (tree[i].type == NODE_PIPE)
			was_pipe = true;
		i++;
	}
	restore_fd(std);
}
