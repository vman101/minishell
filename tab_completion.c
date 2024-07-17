/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:53:58 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 20:03:51 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>

static void	handle_tab_no_match(const char *input_path, const char **env)
{
	uint32_t		cursor_position_stored[2];

	cursor_position_get(cursor_position_stored);
	ft_printf(SCREEN_CLEAR_TO_EOF);
	ft_printf(CURSOR_POSITION_SET, cursor_position_stored[0] + 1, 0);
	command_execute("/usr/bin/ls", (const char *[]){"ls", input_path, NULL}, env);
	ft_printf(CURSOR_POSITION_SET, cursor_position_stored[0], cursor_position_stored[1]);
}

static char	*determine_word(const char *input, char **input_path)
{
	char			*current_word;
	char			*current_word_path_end;

	if (!input || !*input)
		return (NULL);
	current_word = ft_strrchr(input, ' ');
	current_word_path_end = ft_strrchr(input, '/');
	if (current_word && !current_word_path_end)
		return (current_word + 1);
	else if (!current_word && current_word_path_end)
	{
		*input_path = ft_substr(input, 0, current_word_path_end - input);
		return (current_word_path_end + 1);
	}
	else if (current_word && current_word_path_end)
	{
		if (current_word_path_end > current_word)
		{
			*input_path = ft_substr(current_word + 1, 0, current_word_path_end - current_word);
			return (current_word_path_end + 1);
		}
		else
			return (current_word + 1);
	}
	return ((char *)input);
}

uint32_t	find_last_matching_char(const char *current_word, const char *next_word_match)
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

static char *find_next_match(char *current_word, uint32_t current_word_length, DIR *directory_current)
{
	struct	dirent	*directory_entry;
	char			*next_word_match;

	next_word_match = NULL;
	if (!current_word || !directory_current || !*current_word)
		return (NULL);
	while (1)
	{
		directory_entry = readdir(directory_current);
		if (directory_entry == NULL)
			break ;
		if (ft_strncmp(current_word, directory_entry->d_name, current_word_length) == 0)
			next_word_match = directory_entry->d_name;
	}
	return (next_word_match);
}

uint32_t	get_current_word_length(char *word)
{
	char	*word_end;

	if (!word)
		return (0);
	word_end = ft_strchr(word, ' ');
	if (word_end)
		return (word_end - word);
	return (ft_strlen(word));
}

uint32_t	handle_tab(char **input, const char **env, uint32_t *cursor_position_current)
{
	DIR				*directory_current;
	char			*current_word;
	char			*next_word_match;
	char			*input_path;
	uint32_t		current_word_length;

	input_path = NULL;
	current_word = determine_word(*input, &input_path);
	current_word_length = get_current_word_length(current_word);
	if (input_path)
		ft_opendir(&directory_current, input_path);
	else
		ft_opendir(&directory_current, "./");
	next_word_match = find_next_match(current_word, current_word_length, directory_current);
	if (!next_word_match)
		return (closedir(directory_current), handle_tab_no_match(input_path, env), ft_free((void **)&input_path), ft_strlen(*input));
	prompt_string_insert(next_word_match + current_word_length, input, current_word + current_word_length, ft_strlen(next_word_match));
	(*cursor_position_current) += ft_strlen(next_word_match) - current_word_length;
	closedir(directory_current);
	ft_free((void **)&input_path);
	return (ft_strlen(*input));
}

void	prompt_string_insert(char *string_to_insert, char **current_input, char *position_to_insert, uint32_t size_of_to_insert)
{
	uint32_t	current_input_length;

	current_input_length = ft_strlen(*current_input);
	prompt_buffer_size_manage(current_input, size_of_to_insert + current_input_length + 1, PROMPT_INPUT_BUFFER_SIZE);
	ft_memmove(position_to_insert + size_of_to_insert, position_to_insert + 1, size_of_to_insert);
	ft_strlcpy(position_to_insert, string_to_insert, size_of_to_insert);
}
