/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/08/05 17:19:35 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/ioctl.h>

int32_t	g_signal_flag;

void	setup(char **path_variable)
{
	g_signal_flag = 0;
	*path_variable = getenv("PATH");
	if (!path_variable)
		return (p_stderr(2, \
				"No Valid Path Variable was Found in Environment.\nExiting\n", \
				NULL), exit (1));
	setup_signal_handlers();
}

int32_t minishell_single_command(char *command, char **environment, char *path_variable)
{
	m_tokenizer(command, (const char **)environment, path_variable);
	return (0);
}

int	main(int argc, char **argv, const char **env)
{
	char		*path_variable;
	char		*command_input;
	char		**environment;

	setup(&path_variable);
	environment = environment_create(env);
	if (argc > 1)
	{
		if (argc > 2)
			return (-1);
		else
			return (minishell_single_command(argv[1], environment, path_variable));
	}
	while (1)
	{
		command_input = prompt_get((const char **)environment);
		if (g_signal_flag == 1 || !command_input)
		{
			if (g_signal_flag == 1)
				g_signal_flag = 0;
			ft_free(&command_input);
			continue ;
		}
		if (command_input)
		{
			lst_memory(command_input, free, ADD, 0);
			m_tokenizer(command_input, (const char **)environment, path_variable);
		}
	}
}
