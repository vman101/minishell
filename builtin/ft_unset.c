/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:29:41 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/10 22:25:27 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(char **envivonment, const char **args, int32_t *exit_status)
{
	uint32_t	args_size;
	uint32_t	i;

	args_size = get_split_size(args);
	if (args_size < 2)
		return ;
	else if (args[1] && *args[1] == '-')
		return (ft_putendl_fd("unset: usage: unset [name ...]", \
				*exit_status = 2));
	i = 1;
	while (args[i])
	{
		environment_variable_remove(envivonment, args[i]);
		i++;
	}
	*exit_status = 0;
}
