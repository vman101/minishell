/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 00:06:08 by andrejarama       #+#    #+#             */
/*   Updated: 2024/07/16 16:54:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(char **tokens, int i, int tokens_count, int flags, int fd_new)
{
	int		fd;

	(void)fd_new;
	if ((i == 0 && tokens_count == 1) || tokens[i + 1] == NULL)
	{
		perror("minishell: syntax error near unexpected token `newline'");
		lst_memory(NULL, NULL, CLEAN);
	}
	ft_open(&fd, tokens[i + 1], flags, 0644);
	ft_dup2(fd, fd_new, "Redirections");
	ft_close(fd, "Redirections. fd");
	free(tokens[i]);
	tokens[i] = NULL;
	free(tokens[i + 1]);
	tokens[i + 1] = NULL;
	custom_memmove_strings(tokens + i, tokens + i + 2);
}

void	handle_heredoc(char *delimiter, char **tokens, int i, int tokens_count)
{
	char	*line;
	int		pipefd[2];

	if ((i == 0 && tokens_count == 1) || tokens[i + 1] == NULL)
	{
		perror("minishell: syntax error near unexpected token `newline'");
		lst_memory(NULL, NULL, CLEAN);
	}
	if (pipe(pipefd) == -1) 
	{
        perror("pipe");
        lst_memory(NULL, NULL, CLEAN);
    }
	line = NULL;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			perror("Readline failed!");
			lst_memory(NULL, NULL, CLEAN);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
        free(line);
	}
	ft_close(pipefd[1], "Redirections. pipefd[1]");
    if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		close(pipefd[0]);
		perror("Dup2 failed. Redirections");
		lst_memory(NULL, NULL, CLEAN);
	}
    ft_close(pipefd[0], "Redirections. pipefd[0]");
	free(tokens[i]);
	tokens[i] = NULL;
	free(tokens[i + 1]);
	tokens[i + 1] = NULL;
	custom_memmove_strings(tokens + i, tokens + i + 2);
}

void	exec_command(char **tokens) // i know that you have this function already. i will use it instead of this one
{
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed. Redirections");
		lst_memory(NULL, NULL, CLEAN);
	}
	else if (pid == 0)
	{
		execvp(tokens[0], tokens);
		perror("execvp. redirections");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

void	check_redirections(char **tokens, char **env)
{
	int		i;
	int		tokens_count;

	i = 0;
	tokens_count = get_tokens_count(tokens);
	if (tokens_count == 0)
		return ;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{
			handle_dollar_sign(&tokens[i], env);
		}
		else if (ft_strncmp(tokens[i], ">", ft_strlen(tokens[i])) == 0)
		{
			handle_redirection(tokens, i, tokens_count, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
			i = 0;
		}
		else if (ft_strncmp(tokens[i], "<", ft_strlen(tokens[i])) == 0)
		{
			handle_redirection(tokens, i, tokens_count, O_RDONLY, STDIN_FILENO);
			i = 0;
		}
		else if (ft_strncmp(tokens[i], ">>", ft_strlen(tokens[i])) == 0)
		{
			handle_redirection(tokens, i, tokens_count, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
			i = 0;
		}
		else if (ft_strncmp(tokens[i], "<<", ft_strlen(tokens[i])) == 0)
		{
			handle_heredoc(tokens[i + 1], tokens, i, tokens_count);
			i = 0;
		}
		i++;
	}
}
