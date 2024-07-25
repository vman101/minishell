/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:23:40 by anarama           #+#    #+#             */
/*   Updated: 2024/07/25 13:45:03 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_destroy(void *prompt)
{
	t_prompt	*prompt_ptr;

	prompt_ptr = (t_prompt *)prompt;
	ft_free((void **)&prompt_ptr->history_entries);
	ft_free((void **)&prompt_ptr);
}

static uint32_t	prompt_display(const char **environment)
{
	char		*pwd;
	uint32_t	prompt_length;

	pwd = environment_variable_get("PWD", environment);
	prompt_length = ft_strlen(pwd) + 4;
	ft_putstr_fd(GREEN, 1);
	ft_printf("[%s$] ", pwd);
	ft_putstr_fd(RESET, 1);
	return (prompt_length);
}

char	*prompt_get(t_prompt *prompt)
{
	terminal_raw_mode_enable();
	prompt->prompt_length = prompt_display((const char **)prompt->env_ptr);
	prompt_get_input(prompt);
	if (!prompt->command || !*prompt->command)
		return (NULL);
	terminal_raw_mode_disable();
	return (ft_strdup(prompt->command));
}

t_prompt	*prompt_create(const char **env)
{
	t_prompt	*tmp;

	tmp = ft_calloc(1, sizeof(*tmp));
	if (!tmp)
		return (NULL);
	tmp->history_entries = ft_calloc(1024, sizeof(*tmp->history_entries));
	if (!tmp->history_entries)
	{
		ft_free((void **)&tmp);
		return (NULL);
	}
	tmp->history_count = 0;
	tmp->env_ptr = (char **)env;
	lst_memory(tmp, prompt_destroy, ADD);
	return (tmp);
}
