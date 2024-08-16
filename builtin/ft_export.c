/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:41:32 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/17 00:04:44 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>
#include <sys/types.h>

static void	print_environment_a_la_export(char **env)
{
	char		*variable_name;
	char		*variable_value;
	uint32_t	i;

	i = 0;
	while (env[i])
	{
		variable_name = env[i];
		variable_value = ft_strchr(variable_name, '=');
		if (variable_value)
		{
			*variable_value++ = 0;
			ft_printf("declare -x %s=\"%s\"\n", variable_name, variable_value);
			*--variable_value = '=';
		}
		else
			ft_printf("declare -x %s=\"\"\n", variable_name);
		i++;
	}
}

static void	environment_print_sorted(const char **environment)
{
	uint32_t	i;
	uint32_t	length1;
	char		*tmp;

	i = 0;
	while (environment[i] && environment[i + 1])
	{
		tmp = ft_strchr(environment[i], '=');
		if (!tmp)
			tmp = ft_strchr(environment[i], 0);
		length1 =  tmp - environment[i];
		if (ft_strncmp(environment[i], environment[i + 1], length1) > 0)
		{
			tmp = (char *)environment[i];
			environment[i] = environment[i + 1];
			environment[i + 1] = tmp;
			environment_print_sorted(environment);
			break ;
		}
		i++;
	}
	if (environment[i + 1] == NULL)
		print_environment_a_la_export((char **)environment);
}

static void	print_sorted(const char **environment)
{
	char		**env;
	uint32_t	i;

	env = ft_calloc(get_split_size(environment) + 1, sizeof(*env));
	if (!env)
		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN));
	i = -1;
	while (environment[++i])
		env[i] = (char *)environment[i];
	environment_print_sorted((const char **)env);
	ft_free(&env);
}

/*static void	environment_print_sorted(const char **environment)*/
/*{*/
/*	char		**tmp_env;*/
/*	uint{
 *
 *	}32_t	i;*/
/*	uint32_t	split_size;*/
/*	char		*tmp;*/
/**/
/*	split_size = get_split_size(environment);*/
/*	tmp_env = ft_calloc(split_size + 1, sizeof(*tmp_env));*/
/*	if (!tmp_env)*/
/*		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN));*/
/*	i = -1;*/
/*	while (environment[++i])*/
/*		tmp_env[i] = (char *)environment[i];*/
/*	while (split_size)*/
/*	{*/
/*		i = -1;*/
/*		while (++i < split_size - 1)*/
/*		{*/
/*			if (ft_strncmp(tmp_env[i], tmp_env[i + 1], \*/
/*							ft_strlen(tmp_env[i])) > 0)*/
/*			{*/
/*				tmp = tmp_env[i];*/
/*				tmp_env[i] = tmp_env[i + 1];*/
/*				tmp_env[i + 1] = tmp;*/
/*			}*/
/*		}*/
/*		split_size--;*/
/*	}*/
/*	print_environment_a_la_export(tmp_env);*/
/*}*/
/**/

void	ft_export(const char **args, int32_t *exit_status)
{
	uint32_t	args_size;
	uint32_t	i;
	char		*variable_name;
	char		*variable_value;
	char		**environment;

	environment = env_static(NULL);
	args_size = get_split_size(args);
	if (args_size == 1)
		return (print_sorted((const char **)environment));
	i = 1;
	while (args[i])
	{
		variable_name = (char *)args[i];
		variable_value = ft_strchr(args[i], '=');
		if (variable_value)
			*variable_value++ = 0;
		if (!environment_variable_value_get(variable_name, (const char **)environment))
			environment_variable_add(&environment, variable_name, variable_value);
		else
			environment_variable_value_change((const char **)environment, variable_name, variable_value);
		i++;
	}
	*exit_status = 0;
}
