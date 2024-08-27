/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables_manip.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:33:41 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/27 15:12:46 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	environment_buffer_extend(	char ***environment, \
									uint32_t environment_length_current)
{
	uint32_t	environment_size_multiplier;
	void		*env_ptr;

	environment_size_multiplier = environment_length_current / ENVIRONMENT_SIZE;
	if ((ENVIRONMENT_SIZE * (environment_size_multiplier \
		+ (environment_size_multiplier < 1))) \
		< environment_length_current)
	{
		env_ptr = environment;
		*environment = environment_create((const char **)env_ptr);
		lst_memory(env_ptr, NULL, FREE);
		lst_memory(environment, free_split, ADD);
	}
}

char	**environment_create(const char **env)
{
	char		**environment_new;
	char		*pwd;
	uint32_t	i;

	environment_new = ft_calloc(ENVIRONMENT_SIZE + 1, sizeof(*environment_new));
	lst_memory(environment_new, free, ADD);
	i = 0;
	while (env[i])
	{
		environment_new[i] = (char *)env[i];
		i++;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		lst_memory(NULL, NULL, CLEAN);
	}
	environment_variable_value_change((const char **)environment_new, \
									"PWD", pwd);
	ft_free(&pwd);
	return (environment_new);
}

void	clear_spaces(char *variable)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (variable[i])
	{
		j = 0;
		if (variable[i] == ' ')
		{
			while (variable[i + j] && variable[i + j] == ' ')
				j++;
			ft_memmove(&variable[i + 1], \
					&variable[i + j], ft_strlen(&variable[i]));
		}
		i++;
	}
}

void	environment_variable_add(	char ***environment,
									const char *variable_new_name,
									const char *variable_new_value)
{
	char		*variable_to_add;
	uint32_t	variable_new_name_length;
	uint32_t	variable_new_value_length;
	uint64_t	environment_length_current;

	if (environment_variable_get(variable_new_name, \
				(const char **)*environment))
		return (environment_variable_value_change(\
		(const char **)*environment, variable_new_name, variable_new_value));
	environment_length_current = get_split_size((const char **)*environment);
	environment_buffer_extend(environment, environment_length_current);
	variable_new_name_length = ft_strlen(variable_new_name);
	variable_new_value_length = ft_strlen(variable_new_value);
	variable_to_add = ft_calloc(variable_new_name_length \
								+ variable_new_value_length + 2, \
								sizeof(*variable_to_add));
	lst_memory(variable_to_add, free, ADD);
	ft_strlcpy(variable_to_add, variable_new_name, \
				ft_strlen(variable_new_name) + 1);
	ft_strlcpy(variable_to_add + variable_new_name_length, "=", 2);
	if (variable_new_value)
		ft_strlcpy(variable_to_add + variable_new_name_length + 1, \
					variable_new_value, variable_new_value_length + 1);
	clear_spaces(variable_to_add);
	(*environment)[environment_length_current] = variable_to_add;
}

void	environment_variable_remove(char **environment, const char *variable)
{
	uint32_t	i;
	uint32_t	variable_length;

	i = 0;
	variable_length = ft_strlen(variable);
	while (environment[i])
	{
		if (ft_strncmp(environment[i], (char *)variable, variable_length) == 0)
		{
			while (environment[i])
			{
				environment[i] = environment[i + 1];
				i++;
			}
			return ;
		}
		i++;
	}
}
