/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrowkeys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:02:39 by victor            #+#    #+#             */
/*   Updated: 2024/08/26 18:22:57 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdint.h>

void	handle_arrow_key_up(	t_history_buffer *history,
								char **input,
								uint32_t cursor_position_current[2])
{
	if (cursor_position_current[0] > 0)
		ft_putstr_fd(CURSOR_MOVE_UP, 1);
	if (history->read == 0)
		history->read = history->buffer_capacity - 1;
	else
		history->read -= 1;
	if (history->buffer[history->read] == 0)
	{
		history->read = (history->read + 1) % history->buffer_capacity;
		return ;
	}
	if (history->buffer[history->write] == 0)
		history->buffer[history->write] = *input;
	*input = history->buffer[history->read];
	cursor_position_current[1] = ft_strlen(*input);
}

static void	handle_arrow_key_down(	t_history_buffer *history,
									uint32_t cursor_position[2],
									char **input)
{
	if (history->read == history->write)
		return ;
	history->read = (history->read + 1) % history->buffer_capacity;
	*input = history->buffer[history->read];
	cursor_position[1] = ft_strlen(*input);
}

static void	handle_arrow_key_right(	uint32_t *cursor_position_current,
									uint32_t prompt_length_current)
{
	if (cursor_position_current[1] < prompt_length_current)
		cursor_position_current[1]++;
	else
		ft_putstr_fd(CURSOR_MOVE_LEFT, 1);
}

static void	handle_arrow_key_left(	uint32_t *cursor_position_current)
{
	if (cursor_position_current[1] > 0)
		cursor_position_current[1]--;
	else
		ft_putstr_fd(CURSOR_MOVE_RIGHT, 1);
}

bool	handle_escape_sequence(	t_prompt *prompt,
								char buffer[],
								char **input,
								uint32_t cursor_position_current[2])
{
	uint32_t	prompt_length_current;

	prompt_length_current = ft_strlen(*input);
	if (buffer[0] == 91 && buffer[1] == 65)
		return (handle_arrow_key_up(&prompt->history, \
					input, cursor_position_current), 1);
	else if (buffer[0] == 91 && buffer[1] == 66)
		return (handle_arrow_key_down(&prompt->history, \
					cursor_position_current, input), 1);
	else if (buffer[0] == 91 && buffer[1] == 67)
		handle_arrow_key_right(cursor_position_current, prompt_length_current);
	else if (buffer[0] == 91 && buffer[1] == 68)
		handle_arrow_key_left(cursor_position_current);
	return (0);
}
