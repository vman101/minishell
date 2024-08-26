/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:23:40 by anarama           #+#    #+#             */
/*   Updated: 2024/08/26 17:47:31 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_destroy(void *prompt)
{
	t_prompt	*prompt_ptr;

	prompt_ptr = (t_prompt *)prompt;
	ft_free(&prompt_ptr);
}

uint32_t	prompt_display_string_set(	t_prompt *prompt, \
										const char **environment, \
										const char *prompt_string)
{
	char		*pwd;
	uint32_t	prompt_string_length;

	if (!prompt_string)
	{
		pwd = environment_variable_value_get("PWD", environment);
		prompt_string_length = ft_strlen(pwd) + 5;
		prompt->prompt = pwd;
	}
	else
	{
		prompt_string_length = ft_strlen(prompt_string) + 1;
		prompt->prompt = (char *)prompt_string;
	}
	return (prompt_string_length);
}

char	*prompt_get(const char *prompt)
{
	char			*input;
	char			**environment;
	static t_prompt	_prompt = {0};

	environment = env_static(NULL);
	if (!_prompt.exists)
		_prompt = prompt_create(PWD);
	_prompt.prompt = NULL;
	_prompt.prompt_length = prompt_display_string_set(&_prompt, \
										(const char **)environment, \
										prompt);
	prompt_get_input(&_prompt, PROMPT_INPUT_BUFFER_SIZE, "\n");
	if (!_prompt.command || !*_prompt.command)
		return (NULL);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1);
	input = ft_strdup(_prompt.command);
	if (!input)
		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN), NULL);
	lst_memory(input, free, ADD);
	return (input);
}

t_prompt	prompt_create(uint8_t mode)
{
	t_prompt	tmp;

	tmp = (t_prompt){0};
	tmp.history = (t_history_buffer){0};
	tmp.history.buffer_capacity = BUFFER_CAPACITY;
	tmp.exists = true;
	if (mode == PWD)
		tmp.prompt_display_func = prompt_print_pwd;
	else if (mode == CUSTOM)
		tmp.prompt_display_func = prompt_print_custom_string;
	return (tmp);
}
