/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:39:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/04 13:40:05 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	echo_no_newline(char **args)
{
	uint32_t	i;

	i = 2;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
	}
	return (0);
}

int32_t	echo_newline(char **args)
{
	uint32_t	i;

	i = 1;
	while (args[i + 1])
	{
		ft_putstr_fd(args[i], 1);
		ft_putchar_fd(' ', 1);
		i++;
	}
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

	i = 1;
	if (!args[i])
		return (ft_putchar_fd('\n', 1), 0);
	if (ft_strncmp(args[i], "-n", 2) == 0 && (!args[i][2] || args[i][2] == ' '))
		return (echo_no_newline(args), *exit_status = 0);
	else
		return (echo_newline(args), *exit_status = 0);
	return (0);
}
