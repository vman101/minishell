/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_string_management.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:16:45 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 20:03:17 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cursor_position_save(void)
{
	ft_putstr_fd(CURSOR_POSITION_SAVE, 1);
}

void	cursor_position_restore(void)
{
	ft_putstr_fd(CURSOR_POSITION_RESTORE, 1);
}

void	ft_put_substr(const char * string, uint32_t end_position)
{
	uint32_t	i;

	i = 0;
	while (i < end_position)
		ft_putchar_fd(string[i++], 1);
}

void	cursor_position_set(uint32_t row, uint32_t column)
{
	ft_printf(CURSOR_POSITION_SET, row, column);
}

void	prompt_refresh_line(char *input, uint32_t cursor_position_current[2])
{
	cursor_position_restore();
	ft_putstr_fd(input, 1);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOL, 1);
	cursor_position_set(cursor_position_current[0], cursor_position_current[1]);
}

void prompt_buffer_size_manage(char **input, uint32_t input_new_size, int32_t buffer_size)
{
	char		*input_free_ptr;
	uint32_t	buffer_size_multipler;

	buffer_size_multipler = input_new_size / buffer_size;
	if ((buffer_size * (buffer_size_multipler + (buffer_size_multipler < 1))) < input_new_size)
	{
		input_free_ptr = *input;
		*input = ft_calloc(1, buffer_size + input_new_size + 1);
		if (!*input)
			lst_memory(NULL, NULL, CLEAN);
		ft_memcpy(*input, input_free_ptr, input_new_size);
		lst_memory(input_free_ptr, NULL, FREE);
	}
}
