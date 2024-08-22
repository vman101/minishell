/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:39:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/22 10:30:26 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	trim_spaces(char *args)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		j = i;
		while (ft_isspace(args[j]))
			j++;
		ft_memmove(&args[i], &args[j - (j > i && i > 0)], \
				ft_strlen(&args[i]) + 1);
		i++;
	}
}

bool	is_dash_n(char *arg)
{
	uint32_t	i;

	i = 0;
	if (arg[i] && arg[i] == '-')
	{
		while (arg[i] && arg[++i] == 'n')
			;
		if (arg[i] == 0)
			return (true);
	}
	return (false);
}

int32_t	echo_no_newline(char **args)
{
	uint32_t	i;

	i = 0;
	while (args[i] && is_dash_n(args[i]))
		i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (*args[i] && args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	return (0);
}

int32_t	echo_newline(char **args, bool is_single)
{
	uint32_t	i;

	i = 0;
	while (args[i + 1])
	{
		if (is_single)
			trim_spaces(args[i]);
		ft_putstr_fd(args[i], 1);
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (is_single)
		trim_spaces(args[i]);
	if (args[i] && args[i][ft_strlen(args[i]) - \
				(ft_strlen(args[i]) > 0)] == '\n')
		ft_putstr_fd(args[i], 1);
	else
		ft_putendl_fd(args[i], 1);
	return (0);
}

int32_t	ft_echo(char **args, int32_t *exit_status)
{
	uint32_t	i;

	i = 0;
	*exit_status = 0;
	if (args[1])
	{
		if (ft_memcmp(args[1], "-n", 2) == 0)
		{
			while (args[1][++i] == 'n')
				;
			if (args[1][i] == 0)
				return (echo_no_newline(&args[2]), *exit_status = 0);
		}
		echo_newline(&args[1], 0);
		*exit_status = 0;
	}
	else
		ft_putchar_fd('\n', 1);
	return (0);
}
