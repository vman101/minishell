/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:22:34 by victor            #+#    #+#             */
/*   Updated: 2024/08/06 19:12:39 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <sys/ioctl.h>

int32_t	volatile g_signal_flag;

int32_t	minishell_single_command(	const char *command, \
									char **environment, \
									char *path_variable)
{
	char	*tmp;
	int32_t	exit_status;

	exit_status = 0;
	tmp = ft_strrchr(command, '\n');
	if (tmp)
		*tmp = 0;
	m_tokenizer(command, (const char **)environment, path_variable, &exit_status);
	lst_memory(NULL, NULL, END);
	exit(exit_status);
}

void	*ft_realloc_string(char **string, uint32_t *new_size)
{
	char	*tmp;

	tmp = ft_calloc(*new_size, sizeof(*tmp));
	lst_memory(tmp, free, ADD);
	ft_memcpy(tmp, *string, ft_strlen(*string));
	lst_memory(*string, NULL, FREE);
	*string = NULL;
	*new_size += *new_size;
	return (tmp);
}

char	*check_redir_input()
{
	char		*input;
	char		buffer[511];
	uint32_t	capacity;
	int64_t		bytes_read;

	if (isatty(0))
		return (NULL);
	bytes_read = 1;
	capacity = 512;
	input = ft_calloc(capacity, sizeof(*input));
	lst_memory(input, free, ADD);
	while (bytes_read)
	{
		ft_bzero(&buffer, sizeof(buffer));
		bytes_read = read(0, buffer, 511);
		if (bytes_read > 0)
		{
			if ((ft_strlen(input) + ft_strlen(buffer)) >  capacity)
				input = ft_realloc_string(&input, &capacity);
			ft_strlcpy(input, buffer, ft_strlen(input) + ft_strlen(buffer) + 1);
		}
		else if (bytes_read == 0)
			break ;
		else if (bytes_read < 0)
		{
			perror("read");
			lst_memory(NULL, NULL, CLEAN);
		}
	}
	return (input);
}

int	setup(	char **path_variable, \
			uint32_t argc, \
			const char **argv, \
			char **environment)
{
	char	*input;

	g_signal_flag = 0;
	*path_variable = getenv("PATH");
	if (!path_variable)
		return (p_stderr(2, \
		"No Valid Path Variable was Found in Environment.\nExiting\n", \
		NULL), exit (1), 1);
	setup_signal_handlers();
	input = check_redir_input();
	if (input)
		minishell_single_command(input, environment, *path_variable);
	else if (argc > 1)
	{
		if (argc > 2)
			return (-1);
		else
			return (minishell_single_command(argv[1], \
					environment, *path_variable));
	}
	return (1);
}

int	main(int argc, const char **argv, const char **env)
{
	char		*path_variable;
	char		*command_input;
	char		**environment;
	int32_t		exit_status;

	environment = environment_create(env);
	setup(&path_variable, argc, argv, environment);
	exit_status = 0;
	while (1)
	{
		command_input = prompt_get((const char **)environment);
		if (!command_input)
		{
			if (g_signal_flag == 1)
				g_signal_flag = 0;
			else if (g_signal_flag == 2)
			{
				ft_putstr_fd("exit\n", 1);
				lst_memory(NULL, NULL, CLEAN);
			}
			continue ;
		}
		if (command_input)
		{
			lst_memory(command_input, free, ADD);
			m_tokenizer(command_input, (const char **)environment, \
						path_variable, &exit_status);
		}
	}
}