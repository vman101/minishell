/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:41:32 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 11:11:05 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

static void	print_environment_a_la_export(char *variable)
{
	char	*variable_name;
	char	*variable_value;

	variable_name = variable;
	variable_value = ft_strchr(variable, '=');
	if (variable_value)
	{
		*variable_value++ = 0;
		ft_printf("declare -x %s=\"%s\"\n", variable_name, variable_value);
	}
	else
		ft_printf("declare -x %s=\"\"\n", variable_name);
}

static void	environment_print_sorted(const char **environment)
{
	char		**tmp_env;
	uint32_t	i;
	uint32_t	split_size;
	char	*tmp;

	tmp_env = environment_create(environment);
	split_size = get_split_size(environment);
	while (split_size)
	{
		i = 0;
		while (i < split_size - 1)
		{
			if (ft_strncmp(tmp_env[i], tmp_env[i + 1], \
							ft_strlen(tmp_env[i])) > 0)
			{
				tmp = tmp_env[i];
				tmp_env[i] = tmp_env[i + 1];
				tmp_env[i + 1] = tmp;
			}
			i++;
		}
		split_size--;
	}
	i = 0;
	while (tmp_env[i])
		print_environment_a_la_export(tmp_env[i++]);
}

void	variable_exists(const char **environment, const char *variable_name)
{
	uint32_t	i;
	char		*variable_name_terminator;
	uint32_t	variable_name_length;

	variable_name_terminator = ft_strchr(variable_name, '=');
	if (variable_name_terminator)
		*variable_name_terminator = 0;
	variable_name_length = ft_strlen(variable_name);
	i = 0;
	while (environment[i])
	{
		if (!ft_strncmp(environment[i], variable_name, variable_name_length))
			environment_variable_remove((char **)environment, variable_name);
		i++;
	}
	if (variable_name_terminator)
		*variable_name_terminator = '=';
}

void	ft_export(char ***environment, const char **args, int32_t *exit_status)
{
	uint32_t	args_size;
	uint32_t	i;
	char		*variable_name;
	char		*variable_value;

	args_size = get_split_size(args);
	if (args_size == 1)
		return (environment_print_sorted((const char **)*environment));
	i = 1;
	while (args[i])
	{
		variable_name = (char *)args[i];
		variable_value = ft_strchr(args[i], '=');
		if (variable_value)
			*variable_value++ = 0;
		variable_exists((const char **)*environment, variable_name);
		environment_variable_add(*environment, variable_name, variable_value);
		i++;
	}
}
