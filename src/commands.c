/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/07/28 23:01:50 by andrejarama      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	check_valid_logical_operator(t_ast *logical_node, int *error_catched)
{
	if (!logical_node->left || !logical_node->right)
	{
		if (logical_node->token_type == TOKEN_AND)
			printf("minishell: syntax error near unexpected token `&&'\n");
		if (logical_node->token_type == TOKEN_OR)
			printf("minishell: syntax error near unexpected token `||'\n");
		*error_catched = 1;
	}
	else if (logical_node->right->type == NODE_LOGICAL_OPERATOR
		|| logical_node->right->type == NODE_PIPE)
	{
		if (logical_node->right->token_type == TOKEN_AND)
			printf("minishell: syntax error near unexpected token `&&'\n");
		if (logical_node->right->token_type == TOKEN_OR)
			printf("minishell: syntax error near unexpected token `||'\n");
		if (logical_node->right->type == NODE_PIPE)
			printf("minishell: syntax error near unexpected token `|'\n");
		*error_catched = 1;
	}
}

void	traverse_tree(t_ast	*ast, t_ast **head, int *error_catched)
{
	while (ast)
	{
		if (ast->type == NODE_REDIRECTION)
		{
			handle_redir(ast, head, error_catched);
			if (ast->error_found)
			{
				ast = skip_up_to_next_logical_operator(ast);
				continue ;
			}
		}
		else if (ast->type == NODE_PIPE)
		{
			handle_pipe(ast, error_catched);
		}
		else if (ast->type == NODE_LOGICAL_OPERATOR)
		{
			check_valid_logical_operator(ast, error_catched);
		}
		if (*error_catched)
			return ;
		ast = ast->right;
	}
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
	traverse_tree(ast, &ast, &error_catched);
	if (error_catched)
		return (NULL);
	execute_commands(ast, path_variable, env);
	restore_fd(original_stdin, original_stdout);
	return (NULL);
}
