/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/29 16:21:23 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	restore_fd(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

bool	buildin_execute(char const *command, char const **argv)
{
	if (ft_strncmp(command, "exit\0", 5) == 0)
		ft_exit();
	else if (ft_strncmp(command, "cd\0", 3) == 0)
	{
		ft_chdir(argv[1]);
		return (true);
	}
	return (false);
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

void	execute_commands(t_ast *ast, const char *path_variable,
					const char **env)
{
	static int	exit_status;
	t_ast		*current;

	exit_status = 0;
	current = ast;
	while (current)
	{
		if (current->error_found == 1)
		{
			exit_status = 1;
			current = skip_up_to_next_logical_operator(current);
			continue ;
		}
		else if (current->type == NODE_COMMAND && !current->is_done && current->args)
		{
			handle_command(current, path_variable, env, &exit_status);
		}
		else if (current->type == NODE_LOGICAL_OPERATOR)
		{
			handle_logical_operator(current, exit_status);
		}
		current = current->right;
	}
}

int	traverse_tree(t_ast	*ast, t_ast **head)
{
	while (ast)
	{
		if (ast->type == NODE_REDIRECTION)
		{
			handle_redir(ast, head);
			if (ast->error_found)
			{
				ast = skip_up_to_next_logical_operator(ast);
				continue ;
			}
		}
		if (ast->type == NODE_PIPE)
		{
			handle_pipe(ast);
			if (ast->error_found)
			{
				return (0);
			}
		}
		ast = ast->right;
	}
	return (1);
}

int	check_syntax_errors(t_ast *ast)
{
	int error_catched;

	error_catched = 0;
	while (ast)
	{
		if (ast->type == NODE_REDIRECTION)
		{
			check_valid_redir(ast, &error_catched);
		}
		else if (ast->type == NODE_PIPE)
		{
			check_valid_pipe(ast, &error_catched);
		}
		else if (ast->type == NODE_LOGICAL_OPERATOR)
		{
			check_valid_logical_operator(ast, &error_catched);
		}
		if (error_catched)
			return (0);
		ast = ast->right;
	}
	return (1);
}

void	print_tokens(t_token *tokens)
{
	int i;

	i = 0;
	printf("----TOKENS----\n");
	while (tokens[i].token_type != TOKEN_EOL)
	{
		printf("Token: Type=%d, Value=%s\n", tokens[i].token_type, tokens[i].token_value);
		i++;
	}
	printf("------------\n");
}

void	*m_tokenizer(const char *input, const char **env,
			const char *path_variable)
{
	t_token	*tokens;
	t_ast	*ast;
	int		original_stdin;
	int		original_stdout;
	int	error_catched;

	error_catched = 0;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	lst_memory((void *)input, free, ADD);
	tokens = lexical_analysis(input, env);
	ast = parse_tokens(tokens);
	if (!check_syntax_errors(ast))
		return (NULL);
	if (!traverse_tree(ast, &ast))
		return (NULL);
	execute_commands(ast, path_variable, env);
	restore_fd(original_stdin, original_stdout);
	return (NULL);
}
