/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/08/17 15:10:49 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>
#include <stdint.h>

void	restore_fd(int original_stdin, int original_stdout)
{
	ft_close(STDIN_FILENO, "STDIN in restore_fd");
	ft_close(STDOUT_FILENO, "STDOUT in restore_fd");
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
}

void	command_execute(const char *command_path,
						const char **argv,
						const char **env)
{
	pid_t	child_proccess;

	ft_fork(&child_proccess, command_path);
	if (child_proccess == 0)
	{
		execve(command_path, (char **)argv, (char **)env);
		perror("execve");
		lst_memory(NULL, NULL, CLEAN);
	}
	else
	{
		waitpid(child_proccess, NULL, 0);
	}
}

void	print_branch(t_ast *g_tree)
{
	uint32_t	i;

	i = 0;
	while (g_tree[i].type != NODE_END)
	{
		printf("Branch %d\n", i);
		printf("Type: %d\n", g_tree[i].type);
		printf("Connection type: %d\n", g_tree[i].connection_type);
		printf("Has redir in: %d\n", g_tree[i].has_redir_in);
		printf("Has redir out: %d\n", g_tree[i].has_redir_out);
		printf("FD in: %d\n", g_tree[i].fd_in);
		printf("FD out: %d\n", g_tree[i].fd_out);
		printf("Args:\n");
		for (int j = 0; g_tree[i].args[j]; j++)
			printf("%s\n", g_tree[i].args[j]);
		i++;
	}
}

void	execution_loop(	t_ast *tree, \
						const char **env, \
						int *exit_status, \
						int32_t std[2])
{
	uint32_t	i;
	bool		error_found;

	i = 0;
	error_found = false;
	while (tree[i].type != NODE_END)
	{
		if (tree[i].connection_type != TREE_INVALID)
		{
			evaluate_input(&tree[i].args, exit_status, &error_found);
			if (*exit_status == -1 || error_found == true)
			{
				*exit_status = 2;
				break ;
			}
			handle_command(&tree[i], env, exit_status);
			if (tree[i].connection_type != TREE_PIPE)
				restore_fd(std[0], std[1]);
			if ((tree[i].connection_type == TREE_LOGICAL_OR \
				&& *exit_status == 0))
				i++;
			else if ((tree[i].connection_type == TREE_LOGICAL_AND \
				&& *exit_status != 0))
				i++;
		}
		i++;
	}
}

void	execute_commands(t_ast *tree, const char **env, int *exit_status)
{
	int32_t		stdin_org;
	int32_t		stdout_org;

	stdin_org = dup(STDIN_FILENO);
	stdout_org = dup(STDOUT_FILENO);
	if (stdout_org == -1 || stdin_org == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
	execution_loop(tree, env, exit_status, (int [2]){stdin_org, stdout_org});
	ft_close(stdin_org, "stdin in restore_fd");
	ft_close(stdout_org, "stdout in restore_fd");
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

void	*m_tokenizer(const char *input, const char **env, int *exit_status)
{
	t_token	*tokens;
	t_ast	*tree;

	tokens = lexical_analysis((char *)input);
	if (tokens)
	{
		tree = parse_tokens(tokens, env, exit_status);
		if (tree)
			execute_commands(tree, env, exit_status);
		lst_memory(tokens, NULL, FREE);
	}
	return (NULL);
}
