/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:56:00 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/19 23:06:52 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

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
		return (lst_memory(NULL, NULL, CLEAN));
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
		return (lst_memory(NULL, NULL, CLEAN), NULL);
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

void	environment_variable_value_change(	const char **environment, \
											const char *variable_name, \
											const char *variable_new_value)
{
	char		**variable_to_change;
	char		*new_variable;
	uint32_t	new_variable_length;

	variable_to_change = environment_variable_get(variable_name, environment);
	if (!variable_to_change)
		return ;
	new_variable_length = ft_strlen(variable_name) \
							+ ft_strlen(variable_new_value) + 2;
	new_variable = ft_calloc(new_variable_length, sizeof(*new_variable));
	lst_memory(new_variable, free, ADD);
	if (!new_variable)
		lst_memory(NULL, NULL, CLEAN);
	ft_strlcpy(new_variable, variable_name, ft_strlen(variable_name) + 1);
	ft_strlcpy(new_variable + ft_strlen(new_variable), "=", 2);
	ft_strlcpy(new_variable + ft_strlen(new_variable), variable_new_value, \
				ft_strlen(variable_new_value) + 1);
	*variable_to_change = new_variable;
}
