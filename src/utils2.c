/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:50:41 by anarama           #+#    #+#             */
/*   Updated: 2024/08/19 22:57:39 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	custom_memmove_strings(char **dest, char **src)
{
	char	*temp;

	temp = NULL;
	while (*src)
	{
		temp = ft_strdup(*src);
		if (!temp)
		{
			lst_memory(NULL, NULL, CLEAN);
		}
		free(*dest);
		*dest = temp;
		temp = NULL;
		dest++;
		src++;
	}
	while (*dest)
	{
		free(*dest);
		*dest = NULL;
		dest++;
	}
}

int	get_tokens_count(char **tokens)
{
	int	i;

	i = 0;
	while (*tokens)
	{
		tokens++;
		i++;
	}
	return (i);
}

void	*ft_realloc(void *ptr, int old_size, int new_size)
{
	void	*new_ptr;

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
	{
		new_ptr = ft_calloc(new_size, 1);
		return (new_ptr);
	}
	new_ptr = ft_calloc(new_size, 1);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	return (new_ptr);
}
