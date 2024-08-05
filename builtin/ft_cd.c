/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:10:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:41:12 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	pwd_update(const char **environment)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	environment_variable_value_change(environment, "PWD", pwd);
	ft_free(&pwd);
}

int	ft_cd(const char **environment, const char **args)
{
	uint32_t	args_size;
	char		*path_home;

	args_size = get_split_size((const char **)args);
	if (args_size > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 1);	
		return (1);
	}
	else if (args_size == 1)
	{
		path_home = environment_variable_value_get("HOME", environment);
		if (chdir(path_home) != 0)
			return (perror("cd"), 1);
	}
	else
	{
		if (chdir(args[1]) != 0)
			return (perror("cd"), 1);
		pwd_update((const char **)environment);
	}
	return (0);
}
