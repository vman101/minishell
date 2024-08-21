/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/21 14:09:17 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_exit_status(char *status)
{
	uint32_t	i;
	bool		digit_found;
	bool		space_after_digit;

	i = 0;
	digit_found = false;
	space_after_digit = false;
	while (status[i])
	{
		if (!ft_isspace(status[i]) && !ft_isdigit(status[i]))
			return (true);
		if (ft_isalnum(status[i]))
			digit_found = true;
		if (ft_isspace(status[i]) && digit_found)
			space_after_digit = true;
		if (ft_isalnum(status[i]) && space_after_digit)
			return (true);
		i++;
	}
	if (i == 0)
		return (true);
	return (false);
}

bool	exit_with_args(	char **args, \
						bool *invalid_message_print, \
						uint32_t args_length, \
						int32_t *exit_status)
{
	*invalid_message_print = check_exit_status(args[1]);
	if (!*invalid_message_print)
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

uint	get_tree_size(t_ast *tree)
{
	uint	i;

	i = 0;
	while (tree[i].type != NODE_END)
		i++;
	return (i);
}

void	ft_exit(t_ast *tree, int *exit_status_prev)
{
	uint32_t	args_length;
	int32_t		exit_status;
	bool		invalid_message_print;

	exit_status = *exit_status_prev;
	args_length = get_split_size((const char **)tree->args);
	invalid_message_print = false;
	/*ft_putendl_fd("exit", 2);*/
	if (args_length > 1)
		if (exit_with_args(tree->args, &invalid_message_print, \
					args_length, &exit_status))
			return ;
	if (invalid_message_print)
	{
		exit_status = 2;
		p_stderr(2, "minishell: exit: %s: numeric argument required\n", \
				tree->args[1]);
	}
	wait_pids(tree, get_tree_size(tree), tree->cpid, NULL);
	lst_memory(NULL, NULL, END);
	exit(exit_status % 256);
}
