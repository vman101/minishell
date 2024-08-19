/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_string_management.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:16:45 by victor            #+#    #+#             */
/*   Updated: 2024/08/19 23:33:50 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>
#include <sys/types.h>

void	cursor_position_save(void)
{
	ft_putstr_fd(CURSOR_POSITION_SAVE, 1);
}

void	cursor_position_restore(void)
{
	ft_putstr_fd(CURSOR_POSITION_RESTORE, 1);
}

void	cursor_position_set(uint32_t row, uint32_t column)
{
	ft_printf(CURSOR_POSITION_SET, row, column);
}

void	prompt_refresh_line(char *input, \
							uint32_t cursor_position_base, \
							uint32_t cursor_position_current[2])
{
	char		*tmp;
	uint32_t	cursor_position_store;

	cursor_position_store = cursor_position_current[1];
	cursor_position_set(cursor_position_current[0], cursor_position_base);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOL, 1);
	ft_putstr_fd(input, 1);
	cursor_position_set(cursor_position_current[0], \
						cursor_position_current[1] + cursor_position_base);
	cursor_position_current[1] = cursor_position_store;
}

char	*prompt_buffer_size_manage(	char **input, \
									uint32_t old_size, \
									uint32_t size_to_add)
{
	char		*input_free_ptr;
	uint32_t	size_multiplier;

	if ((old_size + size_to_add) > \
			(PROMPT_INPUT_BUFFER_SIZE * \
			((old_size / PROMPT_INPUT_BUFFER_SIZE) + 1)))
	{
		size_multiplier = (old_size / PROMPT_INPUT_BUFFER_SIZE) + 2;
		input_free_ptr = *input;
		*input = ft_calloc(1, size_multiplier * PROMPT_INPUT_BUFFER_SIZE + 1);
		if (!*input)
			return (perror("malloc"), lst_memory(NULL, NULL, CLEAN), NULL);
		ft_memcpy(*input, input_free_ptr, old_size);
		ft_free(&input_free_ptr);
	}
	return (*input);
}
