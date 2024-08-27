/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/27 15:09:15 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_exit_status(char *status)
{
	uint32_t	i;
	char		*number_string;
	int64_t		number;
	uint8_t		is_too_big;

	i = 0;
	if (!status || !*status)
		return (false);
	is_too_big = false;
	while (*status && *status == '0')
		status++;
	if (*status == '+' || *status == '0')
		status++;
	number = ft_atol(status, &is_too_big);
	if (is_too_big == true)
		return (false);
	number_string = ft_ltoa(number);
	if (!number_string)
		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN), 0);
	if (ft_strncmp(status, number_string, ft_strlen(status)) == 0)
		return (ft_free(&number_string), true);
	ft_free(&number_string);
	return (false);
}

bool	exit_with_args(	char **args, \
						bool *invalid_message_print, \
						uint32_t args_length, \
						int32_t *exit_status)
{
	*invalid_message_print = check_exit_status(args[1]);
	if (*invalid_message_print && args_length == 2)
		*exit_status = ft_atol(args[1], \
				(uint8_t *)invalid_message_print);
	else if (!*invalid_message_print)
	{
		*exit_status = 2;
		p_stderr(2, "minishell: exit: %s: numeric argument required\n", \
				args[1]);
	}
	else if (*invalid_message_print && args_length > 2)
		return (ft_putendl_fd("minishell: exit: too many arguments", \
					2), 1);
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
	wait_pids(tree, get_tree_size(tree), tree->cpid, NULL);
	lst_memory(NULL, NULL, END);
	exit(exit_status % 256);
}
