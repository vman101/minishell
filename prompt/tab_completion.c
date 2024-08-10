/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:53:58 by victor            #+#    #+#             */
/*   Updated: 2024/08/10 21:48:04 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/ioctl.h>

void	handle_tab_no_match(const char *input_path, \
								char **env, \
								uint32_t cursor_position_current[2], \
								t_prompt *prompt)
{
	struct winsize	win;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
	cursor_position_save();
	ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 0);
	ft_putstr_fd("\n\r", 1);
	command_execute("/usr/bin/ls", \
					(const char *[]){"ls", input_path, NULL}, \
					(const char **)env);
	cursor_position_restore();
	if (cursor_position_current[0] == win.ws_row)
		prompt->prompt_display_func(prompt->prompt);
}

void	handle_tab_yes_match(	t_prompt *prompt, \
									const char *next_word_match, \
									char **input, \
									uint32_t current_word_length)
{
	struct winsize	win;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
	handle_multiple_character_to_input(input, \
							(char *)(next_word_match + current_word_length), \
							prompt->cursor_position, ft_strlen(*input));
	cursor_position_save();
	ft_putstr_fd("\n\r", 1);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 0);
	cursor_position_restore();
	if (prompt->cursor_position[0] == win.ws_row)
		prompt->prompt_display_func(prompt->prompt);
}

char	*determine_word(char *input, \
							char **input_path, \
							uint32_t cursor_position_current)
{
	char			*current_word;
	char			*current_word_path_end;
	char			*tmp;
	uint32_t		i;

	i = cursor_position_current - (cursor_position_current > 0);
	current_word_path_end = NULL;
	while (i > 0 && input[i] != ' ')
		i--;
	current_word = &input[i + (i > 0)];
	tmp = ft_strchr(current_word, '/');
	while (tmp)
	{
		current_word_path_end = tmp;
		tmp = ft_strchr(tmp + 1, '/');
	}
	if (current_word_path_end)
	{
		*input_path = ft_substr(current_word, 0, \
								current_word_path_end - current_word + 1);
		current_word = current_word_path_end + 1;
		return (current_word);
	}
	return (current_word);
}

uint32_t	find_last_matching_char(const char *current_word, \
									const char *next_word_match)
{
	uint32_t	i;
	char		*current_word_without_path;

	if (!next_word_match)
		return (0);
	current_word_without_path = ft_strrchr(current_word, '/');
	i = 0;
	if (current_word_without_path)
		while ((current_word_without_path)[i] == next_word_match[i])
			i++;
	else
		while (current_word[i] == next_word_match[i])
			i++;
	return (i);
}

void	handle_tab(char **input, t_prompt *prompt)
{
	char			*input_path;
	bool			is_directory;

	if (!(*input)[(prompt->cursor_position)[1] \
			- ((prompt->cursor_position)[1] > 0)])
		return (handle_tab_no_match(".", prompt->env_ptr, \
					prompt->cursor_position, \
					prompt));
	input_path = NULL;
	is_directory = false;
	get_next_word_match(input, prompt, input_path, &is_directory);
}
