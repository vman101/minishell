/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/08/27 17:12:24 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t volatile	g_signal_flag;

int	main(int argc, const char **argv, const char **env)
{
	char		*command_input;
	char		**environment;
	int32_t		exit_status;
	int32_t		std[2];

	environment = environment_create(env);
	setup(argc, argv, environment, std);
	exit_status = 0;
	while (g_signal_flag != 2)
	{
		g_signal_flag = 0;
		env_static(environment);
		command_input = prompt_get(NULL);
		if (g_signal_flag == 2)
			exit_status = 0;
		else if (g_signal_flag == 1)
			exit_status = 130;
		else
			m_tokenizer(command_input, environment, &exit_status, std);
		if (g_signal_flag == 3)
			exit_status = 131;
	}
	ft_putendl_fd("exit", 2);
	return (lst_memory(NULL, NULL, END), exit_status);
}
