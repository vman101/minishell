/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:52:07 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/05 17:29:03 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(char **args, int *exit_status)
{
	int arg_count;

	arg_count = get_tokens_count(args);
	ft_printf("exit\n");
	if (arg_count > 2)
	{
		ft_printf("minishell: exit: too many arguments\n");
		*exit_status = 1;	
	}
	else if (arg_count == 2)
	{
		*exit_status = ft_atoi(args[1]); // handle intmax intmin
	}
	else
		*exit_status = 0;
	lst_memory(NULL, NULL, END, *exit_status);
}
