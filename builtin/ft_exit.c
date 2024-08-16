/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/16 19:05:40 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_exit_status(const char *status)
{
	uint32_t	i;
	bool		digit_found;
	bool		space_after_digit;

	i = 0;
	digit_found = false;
	space_after_digit = false;
	while (status[i])
	{
		if (!ft_isdigit(status[i]))
			return (true);
		else if (ft_isdigit(status[i]))
			digit_found = true;
		if (ft_isspace(status[i]) && digit_found)
			space_after_digit = true;
		if (ft_isdigit(status[i]) && space_after_digit)
			return (true);
		i++;
	}
	return (false);
}

void	ft_exit(const char **args)
{
	uint32_t	args_length;
	int32_t		exit_status;
	bool		invalid_message_print;

	exit_status = 0;
	args_length = get_split_size(args);
	invalid_message_print = false;
	if (args_length > 0)
	{
		invalid_message_print = check_exit_status(args[1]);
		if (!invalid_message_print)
			exit_status = ft_atoi(args[1]);
		else if (!invalid_message_print && args_length > 2)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return ;
		}
	}
	ft_putendl_fd("exit", 2);
	lst_memory(NULL, NULL, END);
	if (invalid_message_print)
	{
		ft_putendl_fd("minshell: exit: numeric arugment required", 2);
		exit(2);
	}
	exit(exit_status);
}
