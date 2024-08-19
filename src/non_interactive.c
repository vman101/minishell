/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:13:31 by victor            #+#    #+#             */
/*   Updated: 2024/08/19 23:51:56 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	minishell_single_command(	char *command, \
									char **environment)
{
	int32_t		exit_status;
	uint32_t	input_length;

	exit_status = 0;
	input_length = ft_strlen(command);
	if (input_length)
	{
		if (command[input_length - 1] == '\n')
			command[input_length - 1] = 0;
		m_tokenizer(command, (const char **)environment, &exit_status);
	}
	lst_memory(NULL, NULL, END);
	exit (exit_status);
}

void	*ft_realloc_string(char **string, uint32_t *new_size)
{
	char	*tmp;

	*new_size += *new_size;
	tmp = ft_calloc(*new_size, sizeof(*tmp));
	lst_memory(tmp, free, ADD);
	ft_memcpy(tmp, *string, ft_strlen(*string));
	lst_memory(*string, NULL, FREE);
	*string = NULL;
	return (tmp);
}

char	*check_redir_input(void)
{
	char		*input;
	char		buffer[512];
	uint32_t	capacity;
	int64_t		bytes_read;

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
			if ((ft_strlen(input) + ft_strlen(buffer)) > capacity)
				input = ft_realloc_string(&input, &capacity);
			ft_strlcpy(input + ft_strlen(input), \
						buffer, ft_strlen(input) + ft_strlen(buffer) + 1);
		}
		else if (bytes_read < 0)
			return (perror("read"), lst_memory(NULL, NULL, CLEAN), NULL);
	}
	return (input);
}

void	setup_environment(const char **environment)
{
	char	*shlvl;

	g_signal_flag = 0;
	env_static((char **)environment);
	setup_signal_handlers();
	shlvl = ft_itoa(ft_atoi(environment_variable_value_get("SHLVL", \
						(const char **)environment)) + 1);
	if (!shlvl)
	{
		perror("malloc");
		lst_memory(NULL, NULL, CLEAN);
	}
	environment_variable_value_change((const char **)environment, \
										"SHLVL", shlvl);
	ft_free(&shlvl);
}

int	setup(	uint32_t argc, \
			const char **argv, \
			char **environment)
{
	char	*input;

	setup_environment((const char **)environment);
	if (!isatty(0))
	{
		input = check_redir_input();
		if (input)
			minishell_single_command(input, environment);
	}
	else if (argc > 1)
	{
		if (argc > 2)
			return (-1);
		else
			return (minishell_single_command((char *)argv[1], \
					environment));
	}
	return (1);
}

