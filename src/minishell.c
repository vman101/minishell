/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/08/19 23:19:18 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t volatile	g_signal_flag;

int	main(int argc, const char **argv, const char **env)
{
	char		*command_input;
	char		**environment;
	int32_t		exit_status;

	environment = environment_create(env);
	setup(argc, argv, environment);
	exit_status = 0;
	while (1)
	{
		command_input = prompt_get((const char **)environment);
		if (command_input)
		{
			lst_memory(command_input, free, ADD);
			m_tokenizer(command_input, \
						(const char **)environment, &exit_status);
			continue ;
		}
		if (g_signal_flag == 1)
			g_signal_flag = 0;
		else if (g_signal_flag == 2)
			break ;
		env_static(environment);
	}
	lst_memory(NULL, NULL, END);
	return (exit_status);
}
