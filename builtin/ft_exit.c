/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/19 23:58:07 by victor           ###   ########.fr       */
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
	if (i == 0)
		return (true);
	return (false);
}

bool	exit_with_args(	const char **args, \
						bool *invalid_message_print, \
						uint32_t args_length, \
						int32_t *exit_status)
{
	*invalid_message_print = check_exit_status(args[1]);
	if (!invalid_message_print)
	{
		if (args_length < 3)
			*exit_status = ft_atol(args[1], \
					(uint8_t *)invalid_message_print);
		else
			return (ft_putendl_fd("minishell: exit: too many arguments", \
						2), 1);
	}
	return (0);
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
		if (exit_with_args(args, &invalid_message_print, args_length, &exit_status))
			return ;
	if (invalid_message_print)
	{
		exit_status = 2;
		p_stderr(2, "minishell: exit: %s: numeric argument required\n", args[1]);
	}
	lst_memory(NULL, NULL, END);
	exit(exit_status % 256);
}
