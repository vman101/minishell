/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_sequences.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:29:29 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 17:05:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cursor_position_get(uint32_t cursor_position[2])
{
	char	cursor_position_str[32];
	char	*cursor_position_str_ptr;
	char	*cursor_position_str_ptr2;
	int		bytes_read;

	ft_bzero(&cursor_position_str, 32);
	write(1, CURSOR_POSITION_GET, ft_strlen(CURSOR_POSITION_GET));
	bytes_read = read(1, cursor_position_str, 32);
	if (bytes_read == -1)
		return (perror("read"));
	cursor_position_str_ptr = cursor_position_str;
	cursor_position_str_ptr2 = ft_strchr(cursor_position_str, ';');
	if (!cursor_position_str_ptr2)
		return ;
	*cursor_position_str_ptr2 = 0;
	while (!(*cursor_position_str_ptr >= '0'
			&& *cursor_position_str_ptr <= '9'))
		cursor_position_str_ptr++;
	cursor_position[0] = ft_atoi(cursor_position_str_ptr);
	cursor_position_str_ptr = cursor_position_str_ptr2 + 1;
	cursor_position_str_ptr2 = ft_strchr(cursor_position_str_ptr, 'R');
	*cursor_position_str_ptr2 = 0;
	cursor_position[1] = ft_atoi(cursor_position_str_ptr);
}
