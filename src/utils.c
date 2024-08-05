/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 12:07:59 by victor            #+#    #+#             */
/*   Updated: 2024/08/05 17:20:12 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_close(int fd, const char *specifier)
{
	if (fd == -1)
		return (0);
	if (close(fd) == -1)
	{
		p_stderr(2, "minishell: %s: ", specifier);
		perror("close");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	return (1);
}

void	ft_pipe(int pipefd[2], const char *specifier)
{
	if (pipe(pipefd) == -1)
	{
		p_stderr(2, "pipex: %s: ", specifier);
		perror("pipe");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
}

void	ft_dup2(int fd_old, int fd_new, const char *specifier)
{
	if (dup2(fd_old, fd_new) == -1)
	{
		p_stderr(2, "pipex: %s: ", specifier);
		perror("dup2");
	}
}

void	ft_fork(pid_t *pid, const char *specifier)
{
	*pid = fork();
	if (*pid == -1)
	{
		p_stderr(2, "pipex: %s: ", specifier);
		perror("fork");
		lst_memory(NULL, NULL, CLEAN, 0);
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
		p_stderr(2, "%s: ", path);
		perror("open");
	}
}

void	ft_opendir(DIR **directory, const char *directory_path)
{
	*directory = opendir(directory_path);
	if (*directory == NULL)
	{
		p_stderr(2, "directory: %s: ", directory_path);
		perror("opendir");
	}
}

int64_t	ft_read(int fd, char *character, char **input, uint32_t size_to_read)
{
	int64_t	bytes_read;

	bytes_read = read(fd, character, size_to_read);
	if (bytes_read == -1)
	{
		if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK || g_signal_flag == 1)
			return (0);
		ft_free(input);
		perror("read");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	return (bytes_read);
}

void	print_debug_string(char *string)
{
	uint32_t	cur_pos[2];

	cursor_position_get(cur_pos);
	ft_printf(CURSOR_POSITION_SET, 80, 80);
	ft_printf("%s", string);
	ft_printf(CURSOR_POSITION_SET, cur_pos[0], cur_pos[1]);
}

void	print_debug_uint(uint32_t info)
{
	uint32_t	cur_pos[2];

	cursor_position_get(cur_pos);
	ft_printf(CURSOR_POSITION_SET, 100, 100);
	ft_printf("%u", info);
	ft_printf(CURSOR_POSITION_SET, cur_pos[0], cur_pos[1]);
}
