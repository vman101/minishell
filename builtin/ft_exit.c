/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/13 10:32:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_exit_status(const char *status)
{
	uint32_t	i;

	i = 0;
	while (ft_isspace(status[i]))
		i++;
	if (!ft_isdigit(status[i]))
		return (false);
	while (status[i] && ft_isdigit(status[i]))
		i++;
	while (status[i] && ft_isspace(status[i]))
		i++;
	if (status[i] != 0)
		return (false);
	return (true);
}

void	ft_exit(const char **args)
{
	uint32_t	args_length;
	int32_t		exit_status;

	exit_status = 0;
	args_length = get_split_size(args);
	if (args_length == 2)
	{
		lst_memory(NULL, NULL, END);
		exit_status = ft_atoi(args[1]);
	}
	ft_putendl_fd("exit", 2);
	exit(exit_status);
}
