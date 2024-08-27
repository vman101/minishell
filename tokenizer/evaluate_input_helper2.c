/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluate_input_helper2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:21:09 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/27 15:21:20 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	evaluate_input(	char **input[], \
						int32_t *exit_status, \
						bool *error_caught)
{
	uint32_t	i;
	uint32_t	j;
	bool		dummy;

	i = 0;
	dummy = false;
	if (!error_caught)
		error_caught = &dummy;
	*error_caught = evaluate_variable(input, exit_status);
	while (*input && (*input)[i] && !*error_caught)
	{
		j = -1;
		while ((*input)[i][++j] && !*error_caught)
		{
			if ((*input)[i][j] == '\"')
				*error_caught = evaluate_double_quotes((*input)[i], &j);
			else if ((*input)[i][j] == '\'')
				*error_caught = evaluate_single_quotes((*input)[i], &j);
		}
		i++;
	}
}
