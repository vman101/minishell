/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_get_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:00:19 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 18:22:17 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_next_match(	char *current_word, \
								uint32_t current_word_length, \
								DIR *directory_current, \
								bool *is_directory)
{
	struct dirent	*directory_entry;
	char			*next_word_match;

	*is_directory = false;
	next_word_match = NULL;
	if (!current_word || !directory_current || !*current_word)
		return (NULL);
	while (1)
	{
		directory_entry = readdir(directory_current);
		if (directory_entry == NULL)
			break ;
		if (ft_strncmp(current_word, \
						directory_entry->d_name, \
						current_word_length) == 0)
		{
			next_word_match = directory_entry->d_name;
			if (directory_entry->d_type == DT_DIR)
				*is_directory = true;
		}
	}
	return (next_word_match);
}

uint32_t	get_current_word_length(char *word)
{
	char	*word_end;

	if (!word)
		return (0);
	else if (*word == ' ')
		return (1);
	word_end = ft_strchr(word, ' ');
	if (word_end)
		return (word_end - word);
	return (ft_strlen(word));
}

void	get_next_word_match(	char **input, \
								t_prompt *prompt, \
								char *input_path, \
								bool *is_directory)
{
	DIR				*directory_current;
	uint32_t		current_word_length;
	char			*next_word_match;
	char			*current_word;

	current_word = determine_word(*input, &input_path, \
			prompt->cursor_position[1]);
	current_word_length = get_current_word_length(current_word);
	if (input_path)
		directory_current = opendir(input_path);
	else
		directory_current = opendir("./");
	next_word_match = find_next_match(current_word, current_word_length, \
										directory_current, is_directory);
	if (!next_word_match)
		handle_tab_no_match(input_path, env_static(NULL), \
				prompt->cursor_position, prompt);
	else
		handle_tab_yes_match(prompt, next_word_match, input, \
							ft_strlen(current_word));
	if (*is_directory)
		handle_new_character_to_input(input, '/', prompt->cursor_position, \
									ft_strlen(*input));
	closedir(directory_current);
	ft_free(&input_path);
}
