/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:10:08 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/19 23:32:21 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	pwd_update(const char **environment)
{
	char	*pwd;
	char	*old_pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		lst_memory(NULL, NULL, CLEAN);
	}
	old_pwd = environment_variable_value_get("PWD", environment);
	environment_variable_value_change(environment, "OLDPWD", old_pwd);
	environment_variable_value_change(environment, "PWD", pwd);
	ft_free(&pwd);
}

void	ft_cd(	const char **environment, \
				const char **args, \
				int32_t *exit_status)
{
	uint32_t	args_size;
	char		*path;

	args_size = get_split_size((const char **)args);
	if (args_size > 2)
		return (*exit_status = 1, \
				ft_putendl_fd("minishell: cd: too many arguments", 2));
	else if (args_size == 1)
	{
		path = environment_variable_value_get("HOME", environment);
		if (chdir(path) != 0)
			return (*exit_status = 1, perror("cd"));
	}
	else
	{
		if (ft_memcmp((void *)args[1], "-\0", 2) == 0)
			path = environment_variable_value_get("OLDPWD", environment);
		else
			path = (char *)args[1];
		if (chdir(path) != 0)
			return (perror(path), \
					*exit_status = 1, ft_putstr_fd("minishell: cd: ", 2));
		*exit_status = 0;
	}
	pwd_update((const char **)environment);
}
