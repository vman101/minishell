/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:56:00 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:19:29 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**environment_create(const char **env)
{
	char		**environment_new;
	char		*pwd;
	uint32_t	i;

	environment_new = ft_calloc(ENVIRONMENT_SIZE + 1, sizeof(*environment_new));
	lst_memory(environment_new, free, ADD, 0);
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
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	environment_variable_value_change((const char **)environment_new, "PWD", pwd);
	ft_free(&pwd);
	return (environment_new);
}

void	environment_print(const char **environment)
{
	uint32_t	i;

	i = 0;
	while (environment[i])
	{
		ft_putendl_fd((char *)environment[i], 1);
		i++;
	}
}

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
		lst_memory(env_ptr, NULL, FREE, 0);
		lst_memory(environment, free_split, ADD, 0);
	}
}

char	**environment_variable_add(	char **environment,
									const char *variable_new_name,
									const char *variable_new_value)
{
	char		*variable_to_add;
	uint32_t	variable_new_name_length;
	uint32_t	variable_new_value_length;
	uint64_t	environment_length_current;

	environment_length_current = get_split_size((const char **)environment);
	if (!environment_length_current)
		return (environment);
	environment_buffer_extend(&environment, environment_length_current);
	variable_new_name_length = ft_strlen(variable_new_name);
	variable_new_value_length = ft_strlen(variable_new_value);
	variable_to_add = ft_calloc(variable_new_name_length \
								+ variable_new_value_length + 2, \
								sizeof(*variable_to_add));
	lst_memory(variable_to_add, free, ADD, 0);
	ft_strlcpy(variable_to_add, variable_new_name, \
				ft_strlen(variable_new_name) + 1);
	ft_strlcpy(variable_to_add + variable_new_name_length, "=", 2);
	if (variable_new_value)
		ft_strlcpy(variable_to_add + variable_new_name_length + 1, \
					variable_new_value, variable_new_value_length + 1);
	environment[environment_length_current] = variable_to_add;
	return (environment);
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

void	environment_variable_print(	const char *variable, \
									const char **environment)
{
	uint32_t	i;
	uint64_t	variable_length;
	char		*variable_findable_name;

	if (!variable || !environment)
		return ;
	i = 0;
	variable_length = ft_strlen(variable);
	variable_findable_name = ft_calloc(variable_length + 2, sizeof(*variable));
	if (!variable_findable_name)
		return (lst_memory(NULL, NULL, CLEAN, 0));
	ft_strlcpy(variable_findable_name, variable, variable_length + 1);
	ft_strlcpy(variable_findable_name + variable_length, "=", 2);
	variable_length = ft_strlen(variable_findable_name);
	while (environment[i])
	{
		if (ft_strncmp(variable_findable_name, environment[i], \
						variable_length) == 0)
		{
			ft_putendl_fd((char *)environment[i] + variable_length, 1);
			break ;
		}
		i++;
	}
	ft_free((void **)&variable_findable_name);
}

char	*environment_variable_value_get( const char *variable, \
									const char **environment)
{
	uint32_t	i;
	uint64_t	variable_length;

	if (!variable || !environment)
		return (NULL);
	i = 0;
	variable_length = ft_strlen(variable);
	while (environment[i])
	{
		if (ft_strncmp(variable, environment[i], \
						variable_length) == 0)
			if (environment[i][variable_length] == '=')
				return ((char *)environment[i] + variable_length + 1);
		i++;
	}
	return (NULL);
}

char	**environment_variable_get( const char *variable, \
									const char **environment)
{
	uint32_t	i;
	uint64_t	variable_length;
	char		*variable_findable_name;

	if (!variable || !environment)
		return (NULL);
	i = 0;
	variable_length = ft_strlen(variable);
	variable_findable_name = ft_calloc(variable_length + 2, sizeof(*variable));
	if (!variable_findable_name)
		return (lst_memory(NULL, NULL, CLEAN, 0), NULL);
	ft_strlcpy(variable_findable_name, variable, variable_length + 1);
	ft_strlcpy(variable_findable_name + variable_length, "=", 2);
	variable_length = ft_strlen(variable_findable_name);
	while (environment[i])
	{
		if (ft_strncmp(variable_findable_name, environment[i], \
						variable_length) == 0)
			return (ft_free(&variable_findable_name), (char **)&environment[i]);
		i++;
	}
	ft_free(&variable_findable_name);
	return (NULL);
}

void	environment_variable_value_change(const char **environment, const char *variable_name, const char *variable_new_value)
{
	char		**variable_to_change;
	char		*new_variable;
	uint32_t	new_variable_length;

	variable_to_change = environment_variable_get(variable_name, environment);
	if (!variable_to_change)
		return ;
	new_variable_length = ft_strlen(variable_name) + ft_strlen(variable_new_value) + 2;
	new_variable = ft_calloc(new_variable_length, sizeof(*new_variable));
	lst_memory(new_variable, free, ADD, 0);
	if (!new_variable)
		lst_memory(NULL, NULL, CLEAN, 0);
	ft_strlcpy(new_variable, variable_name, ft_strlen(variable_name) + 1);
	ft_strlcpy(new_variable + ft_strlen(new_variable), "=", 2);
	ft_strlcpy(new_variable + ft_strlen(new_variable), variable_new_value, ft_strlen(variable_new_value) + 1);
	*variable_to_change = new_variable;
}
