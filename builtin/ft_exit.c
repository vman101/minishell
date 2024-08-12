/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/12 11:59:55 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_input(char *str, int num)
{
	char	*check;

	check = ft_itoa(num);
	if (!check || ft_strncmp(check, str, ft_strlen(str)) != 0)
	{
		free(check);
		return (0);
	}
	free(check);
	return (1);
}

void	ft_exit(char **args, int *exit_status)
{
	int arg_count;

	arg_count = get_tokens_count(args);
	ft_printf("exit\n");
	if (arg_count > 2)
	{
		ft_printf("minishell: exit: too many arguments\n");
		*exit_status = 1;
		return ;
	}
	else if (arg_count == 2)
	{
		*exit_status = ft_atoi(args[1]);
		if (!check_input(args[1], *exit_status))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", args[1]);
			*exit_status = 2;
		}
	}
	else
		*exit_status = 0;
	lst_memory(NULL, NULL, END, *exit_status);
}
