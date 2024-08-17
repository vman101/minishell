/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:39:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/17 17:58:57 by victor           ###   ########.fr       */
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
		ft_memmove(&args[i], &args[j - (j > i && i > 0)], ft_strlen(&args[i]) + 1);
		i++;
	}
}

int32_t	echo_no_newline(char **args)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		if (args[i][j] == 'n')
			while (args[i][j] == 'n')
				j++;
		if (args[i][j] != 0 && args[i][j] != ' ')
			break ;
		i++;
	}
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
	/*trim_spaces(args[0]);*/
	if (!args[1])
	{
		if (ft_memcmp(args[0], "echo", ft_strlen("echo")) == 0 && args[0][4] == ' ')
		{
			args[0] = &args[0][4];
			if (ft_memcmp(args[0], "-n", 2) == 0)
			{
				if (args[i][2] == ' ' || args[i][2] == 0)
					return (args[i] = &args[i][2 + (args[i][2] == ' ')], echo_no_newline(args), *exit_status = 0);
			}
			else
				return (echo_newline(args, 1), *exit_status = 0, 1);
		}
		else
			ft_putchar_fd('\n', 1);
	}
	i = 1;
	if (args[i] && ft_memcmp(args[i], "-n", 2) == 0) 
	{
		while (args[1][i] && args[1][i] == 'n')
			i++;
		if (args[1][i] == 0)
			return (echo_no_newline(&args[1]), *exit_status = 0);
		return (echo_newline(&args[1], 0), *exit_status = 0);
	}
	else
		return (echo_newline(&args[1], 0), *exit_status = 0);
	return (0);
}
