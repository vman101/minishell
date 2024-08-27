/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:13:31 by victor            #+#    #+#             */
/*   Updated: 2024/08/27 16:58:26 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	minishell_single_command(	char *command, \
									char **environment, \
									int32_t std[2])
{
	int32_t		exit_status;
	uint32_t	input_length;

	exit_status = 0;
	input_length = ft_strlen(command);
	if (input_length)
	{
		if (command[input_length - 1] == '\n')
			command[input_length - 1] = 0;
		m_tokenizer(command, environment, &exit_status, std);
	}
	lst_memory(NULL, NULL, END);
	exit(exit_status);
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
		bytes_read = ft_read(0, buffer, 511);
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
	char		*shlvl;
	uint16_t	shlvl_value;

	g_signal_flag = 0;
	env_static((char **)environment);
	setup_signal_handlers();
	shlvl_value = ft_atoi(environment_variable_value_get("SHLVL", \
						(const char **)environment)) + 1;
	if (shlvl_value == 1000)
	{
		ft_putendl_fd("minishell: \
				warning: shell level (1000) too high, resetting to 1", 2);
		shlvl = ft_strdup("1");
	}
	else
		shlvl = ft_itoa(shlvl_value);
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
			char **environment, \
			int32_t std[2])
{
	char	*input;

	setup_environment((const char **)environment);
	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	if (std[0] == -1 || std[1] == -1)
		return (perror("dup"), lst_memory(NULL, NULL, CLEAN), 1);
	lst_memory(std, close_fds, ADD);
	if (!isatty(0))
	{
		input = check_redir_input();
		if (input)
			minishell_single_command(input, environment, std);
	}
	else if (argc > 1)
	{
		if (argc > 2)
			return (-1);
		else
			return (minishell_single_command((char *)argv[1], \
					environment, std));
	}
	return (1);
}
