/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/08/27 17:26:44 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_fd(int std[2])
{
	ft_dup2(std[0], STDIN_FILENO, "in restore_fd");
	ft_dup2(std[1], STDOUT_FILENO, "in restore_fd");
	ft_close(std[0], "in execution");
	ft_close(std[1], "in execution");
	std[0] = dup(STDIN_FILENO);
	std[1] = dup(STDOUT_FILENO);
	if (std[0] == -1 || std[1] == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
}

void	close_fds(void *std_ptr)
{
	int	*std;

	std = std_ptr;
	ft_close(STDIN_FILENO, "close fds");
	ft_close(STDOUT_FILENO, "close fds");
	ft_close(STDERR_FILENO, "close fds");
	ft_close(std[0], "close fds");
	ft_close(std[1], "close fds");
}

void	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	printf("----TOKENS----\n");
	while (tokens[i].token_type != TOKEN_EOL)
	{
		printf("Token: Type=%d, Value=%s\n", \
				tokens[i].token_type, tokens[i].token_value);
		i++;
	}
	printf("------------\n");
}

void	m_tokenizer(	const char *input, char **env, \
						int *exit_status, int32_t std[2])
{
	t_token	*tokens;
	t_ast	*tree;

	if (input)
	{
		tokens = lexical_analysis((char *)input);
		if (tokens)
		{
			tree = parse_tokens(tokens, exit_status);
			if (tree)
				execute_commands(tree, (const char **)env, exit_status, std);
			lst_memory(tokens, NULL, FREE);
		}
	}
}
