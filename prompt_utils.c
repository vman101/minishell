/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:23:40 by anarama           #+#    #+#             */
/*   Updated: 2024/07/19 11:36:16 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

void	prompt_destroy(void *prompt)
{
	t_prompt	*prompt_ptr;

	prompt_ptr = (t_prompt *)prompt;
	ft_free((void **)&prompt_ptr->history_entries);
	ft_free((void **)&prompt_ptr);
}

uint32_t	prompt_display(void)
{
	char		*pwd;
	uint32_t	prompt_length;

	pwd = getcwd(NULL, 0);
	prompt_length = ft_strlen(pwd) + 4;
	ft_printf(GREEN);
	ft_printf("[%s]$ ", pwd);
	ft_printf(RESET);
	ft_free((void **)&pwd);
	return (prompt_length);
}

char	*prompt_get(t_prompt *prompt)
{
	prompt->prompt_length = prompt_display();
	prompt_get_input(prompt);
	if (!prompt->command || !*prompt->command)
		return (NULL);
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
