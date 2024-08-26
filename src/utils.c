/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 12:07:59 by victor            #+#    #+#             */
/*   Updated: 2024/08/24 12:54:28 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**env_static(char **environment)
{
	static char	**env;

	if (environment)
		env = environment;
	return (env);
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

int64_t	ft_read(int fd, char *character, uint32_t size_to_read)
{
	int64_t	bytes_read;

	bytes_read = read(fd, character, size_to_read);
	if (bytes_read == -1)
	{
		if (errno == EINTR \
			|| errno == EAGAIN \
			|| errno == EWOULDBLOCK \
			|| g_signal_flag == 1)
			return (0);
		perror("read");
		lst_memory(NULL, NULL, CLEAN);
	}
	return (bytes_read);
}
