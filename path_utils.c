/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:32:35 by anarama           #+#    #+#             */
/*   Updated: 2024/07/19 16:35:25 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_longest_path(const char *path)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (*path++)
	{
		if (*path == ':')
		{
			if (ret < i)
			{
				ret = i;
				i = 0;
			}
		}
		i++;
	}
	return (ret);
}

static char	*print_error(char *input)
{
	if (input && ft_strchr(input, '/'))
	{
		if (access(input, F_OK) == 0)
		{
			if (access(input, X_OK) == 0)
				return (input);
			else
				p_stderr(STDERR_FILENO, "minishell: %s: permission denied\n", \
						input);
		}
		else
			p_stderr(STDERR_FILENO, \
					"minishell: %s: no such file or directory\n", \
					input);
	}
	else
		p_stderr(STDERR_FILENO, "minishell: %s: command not found\n", input);
	ft_free((void **)&input);
	return (NULL);
}

char	*check_paths(const char *path, char *path_abs, char *input)
{
	while (path)
	{
		ft_strlcpy(path_abs, path, ft_strchr(path, ':') - path + 1);
		ft_strlcat(path_abs, "/", ft_strlen(path_abs) + 2);
		ft_strlcat(path_abs, input, ft_strlen(input) + ft_strlen(path_abs) + 1);
		if (access(path_abs, F_OK) == 0)
		{
			if (access(path_abs, X_OK) == 0)
				return (path_abs);
		}
		path = ft_strchr(path, ':');
		if (path)
			path++;
	}
	return (print_error(input));
}

char	*find_absolute_path(const char *path_variable, char *input)
{
	char	*path_abs;

	if (ft_strchr(input, '/'))
		return (print_error(input));
	path_abs = malloc(find_longest_path(path_variable) + ft_strlen(input) + 2);
	lst_memory(path_abs, &free, ADD);
	return (check_paths(path_variable, path_abs, input));
}
