/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:20:59 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:23:33 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	subshell_child_execute(	char *input, \
									const char **environement, \
									int32_t pipefd[2])
{
	char	*path_variable;

	path_variable = environment_variable_value_get("PATH", environement);
	if (!path_variable)
		return ;
	ft_close(pipefd[0], "pipefd[0] in child parent");
	ft_dup2(pipefd[1], STDOUT_FILENO, "execute_subshell stdout");
	ft_close(pipefd[1], "close in execute_subshell");
	m_tokenizer(input, environement, path_variable);
	lst_memory(NULL, NULL, END, 0);
}

static void	subshell_parent_execute(char *input_subshell, int32_t pipefd[2])
{
	int64_t		bytes_read;
	uint32_t	i;
	char		c;

	ft_close(pipefd[1], "close in execute_subshell");
	i = 0;
	while (1)
	{
		bytes_read = read(pipefd[0], &c, 1);
		if (bytes_read == -1)
		{
			perror("read");
			lst_memory(NULL, NULL, CLEAN, 0);
		}
		else if (bytes_read == 0)
			break ;
		prompt_buffer_size_manage(&input_subshell, i, i * 2);
		input_subshell[i] = c;
		i++;
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
	lst_memory(input_subshell, free, ADD, 0);
	ft_pipe(pipefd, "in execute_subshell");
	ft_fork(&pid, "execute_subshell");
	if (pid == 0)
		subshell_child_execute(input, environement, pipefd);
	else
		subshell_parent_execute(input_subshell, pipefd);
	return (input_subshell);
}
