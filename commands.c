/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 17:02:00 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	buildin_execute(char const *command, char const **argv)
{
	if (ft_strncmp(command, "exit\0", 5) == 0)
		ft_exit();
	else if (ft_strncmp(command, "cd\0", 3) == 0)
	{
		ft_chdir(argv[1]);
		return (true);
	}
	return (false);
}

void	command_execute(char const *command_path,
						char const *argv[],
						char const **env)
{
	pid_t	child_process;

	ft_fork(&child_process, command_path);
	if (child_process == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_process, NULL, 0);
	}
}

void	*m_tokenizer(const char *input, const char **env, const char *path_variable)
{
	char		**input_split;
	char		*command_path;

	input_split = ft_split(input, ' ');
	lst_memory(input_split, &free_split, ADD);
	if (!*input_split)
		return (NULL);
	if (buildin_execute(*input_split, (char const **)input_split))
		return (NULL);
	command_path = find_absolute_path(path_variable, (char *)*input_split);
	if (command_path == 0)
		return (NULL);
	command_execute(command_path, (char const **)input_split, env);
	return (NULL);
}
