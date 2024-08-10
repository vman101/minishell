/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:17:47 by anarama           #+#    #+#             */
/*   Updated: 2024/08/10 22:20:35 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	**custom_realloc(void ***args, int old_capacity, int new_capacity)
{
	void	**new_args;

	new_args = ft_calloc(new_capacity + 1, sizeof(void *));
	if (!new_args)
	{
		perror("Malloc failed while reallocing memory");
		lst_memory(NULL, NULL, CLEAN);
	}
	ft_memcpy(new_args, *args, old_capacity * sizeof(void *));
	free(*args);
	return (new_args);
}

void	fill_args(char ***args, int count, char *token_value, int *capacity)
{
	if (count >= *capacity)
	{
		*args = (char **)custom_realloc((void ***)args,
				*capacity, *capacity * 2);
		if (!args)
		{
			perror("calloc in parse tokens");
			lst_memory(NULL, NULL, CLEAN);
		}
		*capacity *= 2;
	}
	(*args)[count] = token_value;
}
