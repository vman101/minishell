/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:23:40 by anarama           #+#    #+#             */
/*   Updated: 2024/08/02 16:12:19 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_destroy(void *prompt)
{
	t_prompt	*prompt_ptr;

	prompt_ptr = (t_prompt *)prompt;
	ft_free(&prompt_ptr->history_entries);
	ft_free(&prompt_ptr);
}

uint32_t	prompt_display_string_set(	t_prompt *prompt, \
										const char **environment, \
										const char *prompt_string)
{
	char		*pwd;
	uint32_t	prompt_string_length;

	if (environment)
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

char	*prompt_get(const char **environment)
{
	char			*input;
	static t_prompt	prompt = {0};

	if (!prompt.exists)
		prompt = prompt_create(environment, PWD);
	prompt.prompt = NULL;
	prompt.prompt_length = prompt_display_string_set(&prompt, \
													environment, \
													NULL);
	prompt.command = prompt_get_input(&prompt, PROMPT_INPUT_BUFFER_SIZE, "\n");
	if (!prompt.command || !*prompt.command)
		return (NULL);
	if (g_signal_flag == 1 && (!prompt.command || !*prompt.command))
		return (ft_putstr_fd("^C", 1), ft_putchar_fd('\n', 1), NULL);
	ft_putstr_fd(SCREEN_CLEAR_TO_EOF, 1);
	input = ft_strdup(prompt.command);
	if (!input)
		return (perror("malloc"), lst_memory(NULL, NULL, CLEAN), NULL);
	return (input);
}

t_prompt	prompt_create(const char **env, uint8_t mode)
{
	t_prompt	tmp;

	tmp = (t_prompt){0};
	tmp.history_entries = ft_calloc(16, sizeof(*tmp.history_entries));
	lst_memory(tmp.history_entries, free, ADD);
	tmp.history_count = 0;
	tmp.env_ptr = (char **)env;
	tmp.exists = true;
	if (mode == PWD)
		tmp.prompt_display_func = prompt_print_pwd;
	else if (mode == CUSTOM)
		tmp.prompt_display_func = prompt_print_custom_string;
	return (tmp);
}
