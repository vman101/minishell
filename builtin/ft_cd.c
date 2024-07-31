/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:10:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/31 13:34:21 by vvobis           ###   ########.fr       */
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
		lst_memory(NULL, NULL, CLEAN);
	}
	environment_variable_value_change(environment, "PWD", pwd);
	ft_free(&pwd);
}

void	ft_cd(const char **environment, const char **args)
{
	uint32_t	args_size;
	char		*path_home;

	args_size = get_split_size((const char **)args);
	if (args_size > 2)
		return (ft_putendl_fd("minishell: cd: too many arguments", 1));
	else if (args_size == 1)
	{
		path_home = environment_variable_value_get("HOME", environment);
		if (chdir(path_home) != 0)
			return (perror("cd"));
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("cd");
		pwd_update((const char **)environment);
	}
}
