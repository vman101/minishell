/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:20:59 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/24 13:27:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

static void	subshell_child_execute(	char *input, \
									const char **environement, \
									int32_t pipefd[2])
{
	char	*path_variable;
	int32_t	exit_status;

	exit_status = 0;
	path_variable = environment_variable_value_get("PATH", environement);
	if (!path_variable)
		return ;
	ft_close(pipefd[0], "pipefd[0] in child parent");
	ft_dup2(pipefd[1], STDOUT_FILENO, "execute_subshell stdout");
	ft_close(pipefd[1], "close in execute_subshell");
	m_tokenizer(input, (char **)environement, &exit_status, NULL);
	lst_memory(NULL, NULL, END);
	exit(exit_status);
}

void	manage_buffer_size_subshel(char **input_subshell, uint32_t *buffer_size)
{
	uint32_t	buffer_strlen;

	buffer_strlen = ft_strlen(*input_subshell);
	if (buffer_strlen == *buffer_size - 101)
		prompt_buffer_size_manage(input_subshell, *buffer_size, \
								*buffer_size * 2, PROMPT_INPUT_BUFFER_SIZE);
	*buffer_size *= 2;
}

static void	subshell_parent_execute(char **input_subshell, int32_t pipefd[2])
{
	int64_t		bytes_read;
	uint32_t	string_size;

	ft_close(pipefd[1], "close in execute_subshell");
	string_size = 0;
	while (1)
	{
		bytes_read = read(pipefd[0], *input_subshell + string_size, 100);
		if (bytes_read == -1)
		{
			perror("read");
			lst_memory(NULL, NULL, CLEAN);
		}
		else if (bytes_read == 0)
			break ;
		else
			string_size += bytes_read;
	}
	ft_close(pipefd[0], "pipefd[0] in subshell parent");
}

char	*execute_subshell(char *input, const char **environement)
{
	char	*input_subshell;
	int32_t	pipefd[2];
	pid_t	pid;

	input_subshell = NULL;
	input_subshell = ft_calloc(PROMPT_INPUT_BUFFER_SIZE, \
								sizeof(*input_subshell));
	lst_memory(input_subshell, free, ADD);
	ft_pipe(pipefd, "in execute_subshell");
	ft_fork(&pid, "execute_subshell");
	if (pid == 0)
		subshell_child_execute(input, environement, pipefd);
	else
		subshell_parent_execute(&input_subshell, pipefd);
	return (input_subshell);
}
