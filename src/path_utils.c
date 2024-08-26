/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:32:35 by anarama           #+#    #+#             */
/*   Updated: 2024/08/26 18:24:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	find_longest_path(const char *path)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	if (!path)
		return (0);
	while (*path++)
	{
		if (*path == ':' || !*path)
		{
			if (ret < i)
			{
				ret = i;
				i = 0;
			}
		}
		i++;
	}
	if (ret)
		return (ret);
	return (i);
}

bool	isdir(char	*input)
{
	struct stat	dir;

	dir = (struct stat){0};
	stat(input, &dir);
	if (S_ISDIR(dir.st_mode))
		return (true);
	return (false);
}

static char	*print_error(char *input, int *exit_status)
{
	if (input && ft_strchr(input, '/'))
	{
		if (input && isdir(input))
			return (p_stderr(2, "minishell: %s: Is a directory\n", input), \
					*exit_status = 126, NULL);
		if (access(input, F_OK) == 0)
		{
			if (access(input, X_OK) == 0)
				return (input);
			else
			{
				p_stderr(STDERR_FILENO, "minishell: %s: Permission denied\n", \
						input);
				*exit_status = 126;
			}
		}
		else
			return (p_stderr(STDERR_FILENO, \
						"minishell: %s: No such file or directory\n", \
						input), *exit_status = 127, NULL);
	}
	else
		return (*exit_status = 127, p_stderr(STDERR_FILENO, \
					"minishell: %s: command not found\n", input), NULL);
	return (NULL);
}

static char	*check_paths(	const char *path, \
							char *path_abs, \
							char *input, \
							int *exit_status)
{
	char	*tmp;

	if (ft_memcmp(input, ".", 2) != 0 && ft_memcmp(input, "..", 3) != 0)
	{
		while (path)
		{
			tmp = ft_strchr(path, ':');
			if (tmp)
			{
				ft_strlcpy(path_abs, path, tmp - path + 1);
				ft_strlcat(path_abs, "/", ft_strlen(path_abs) + 2);
				ft_strlcat(path_abs, input, \
						ft_strlen(input) + ft_strlen(path_abs) + 1);
			}
			if (access(path_abs, F_OK) == 0)
			{
				if (access(path_abs, X_OK) == 0)
					return (path_abs);
			}
			path = ft_strchr(path, ':');
			if (path)
				path++;
		}
	}
	return (print_error(input, exit_status));
}

char	*find_absolute_path(	const char *path_variable, \
								char *input, \
								int *exit_status)
{
	char		*path_abs;
	uint32_t	path_length;

	if (!input)
		return (NULL);
	if ((input && *input == 0) || ft_strchr(input, '/') || !path_variable)
		return (print_error(input, exit_status));
	path_variable = ft_strchr(path_variable, '/');
	path_length = find_longest_path(path_variable);
	if (!path_length)
		return (NULL);
	path_abs = malloc(path_length + ft_strlen(input) + 2);
	lst_memory(path_abs, &free, ADD);
	return (check_paths(path_variable, path_abs, input, exit_status));
}
