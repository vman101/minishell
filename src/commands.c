/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/08/07 15:20:15 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	execute_commands(t_ast *tree, const char *path_variable,
					const char **env, int *exit_status)
{
	uint32_t	i;
	int32_t		stdin_org;
	int32_t		stdout_org;

	stdin_org = dup(STDIN_FILENO);
	stdout_org = dup(STDOUT_FILENO);
	if (stdout_org == -1 || stdin_org == -1)
	{
		perror("dup");
		lst_memory(NULL, NULL, CLEAN);
	}
	i = 0;
	while (tree[i].type != NODE_END)
	{
		if (tree[i].connection_type == TREE_INVALID)
		{
			i++;
			continue ;
		}
		evaluate_input(&tree->args, env, exit_status, 0);
		if (*exit_status == -1)
		{
			*exit_status = 2;
			break ;
		}
		handle_command(&tree[i], path_variable, env, exit_status);
		if (tree[i].connection_type != TREE_PIPE)
			restore_fd(stdin_org, stdout_org);
		if ((tree[i].connection_type == TREE_LOGICAL_OR && *exit_status == 0))
			i++;
		else if ((tree[i].connection_type == TREE_LOGICAL_AND && *exit_status != 0))
			i++;
		i++;
	}
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

void	*m_tokenizer(const char *input, const char **env,
			const char *path_variable, int *exit_status)
{
	t_token	*tokens;
	t_ast	*tree;

	tokens = lexical_analysis(input, env);
	// handle_heredoc(tokens);
	print_tokens(tokens);
	check_and_expand_wildcards(&tokens);
	print_tokens(tokens);
	tree = parse_tokens(tokens, env, exit_status);
	if (tree)
		execute_commands(tree, path_variable, env, exit_status);
	// tree = parse_tokens(tokens);
	// /*if (error_catched)*/
	// /*	skip_up_to_logical_operator(tree);*/
	// execute_commands(tree, path_variable, env, &error_catched);
	lst_memory(tokens, NULL, FREE);
	return (NULL);
}