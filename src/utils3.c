/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:32:16 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 11:46:25 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

int	ft_close(int fd, const char *specifier)
{
	if (fd == -1)
		return (0);
	if (close(fd) == -1)
	{
		p_stderr(2, "minishell: %s: ", specifier);
		perror("close");
	}
	return (1);
}

void	ft_pipe(int pipefd[2], const char *specifier)
{
	if (pipe(pipefd) == -1)
	{
		p_stderr(2, "minishell: %s: ", specifier);
		perror("pipe");
	}
}

void	ft_dup2(int fd_old, int fd_new, const char *specifier)
{
	if (dup2(fd_old, fd_new) == -1)
	{
		p_stderr(2, "minishell: %s: ", specifier);
		perror("dup2");
	}
}

void	ft_fork(pid_t *pid, const char *specifier)
{
	*pid = fork();
	if (*pid == -1)
	{
		p_stderr(2, "minishell: %s: ", specifier);
		perror("fork");
		lst_memory(NULL, NULL, CLEAN);
	}
}

void	ft_open(int *fd, const char *path, int flag, int mode)
{
	if (mode > 0)
		*fd = open(path, flag, mode);
	else
		*fd = open(path, flag);
	if (*fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(path);
	}
}
