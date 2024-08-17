/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/17 18:40:41 by victor           ###   ########.fr       */
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
	if (args_length > 1)
	{
		invalid_message_print = check_exit_status(args[1]);
		if (!invalid_message_print)
		{
			if (args_length < 3)
				exit_status = ft_atoi(args[1]);
			else
				return (ft_putendl_fd("exit\nminishell: exit: too many arguments", 2));
		}
	}
	if (invalid_message_print)
	{
		p_stderr(2, ": exit: %s: numeric argument required\n", args[1]);
		exit_status = 2;
	}
	else
		ft_putendl_fd("exit", 2);
	lst_memory(NULL, NULL, END);
	exit(exit_status);
}
