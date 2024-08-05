/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/08/05 17:18:28 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	restore_fd(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	ft_close(original_stdin, "stdin in restore_fd");
	ft_close(original_stdout, "stdout in restore_fd");
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
		lst_memory(NULL, NULL, CLEAN, 0);
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

	i = 0;
	while (tree[i].type != NODE_END)
	{
		handle_command(&tree[i], path_variable, env, exit_status);
		if (tree[i].connection_type == TREE_LOGICAL_OR && *exit_status == 0)
			i++;
		else if (tree[i].connection_type == TREE_LOGICAL_AND && *exit_status != 0)
			i++;
		i++;
	}
}
/**/
/*void	check_valid_logical_operator(t_ast *logical_node, int *error_catched)*/
/*{*/
/*	if (!logical_node->left || !logical_node->right)*/
/*	{*/
/*		printf("minishell: syntax error near unexpected token `&&'\n");*/
/*		*error_catched = 1;*/
/*	}*/
/*}*/
/**/
/*void	traverse_tree(t_ast	*ast, t_ast **head, int *error_catched)*/
/*{*/
/*	while (ast)*/
/*	{*/
/*		if (ast->type == NODE_REDIRECTION)*/
/*		{*/
/*			handle_redir(ast, head, error_catched);*/
/*		}*/
/*		else if (ast->type == NODE_LOGICAL_OPERATOR)*/
/*		{*/
/*			check_valid_logical_operator(ast, error_catched);*/
/*		}*/
/*		if (*error_catched)*/
/*			return ;*/
/*		ast = ast->right;*/
/*	}*/
/*}*/

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

/*void	skip_up_to_logical_operator(t_ast *ast)*/
/*{*/
/*	while (ast)*/
/*	{*/
/*		if (ast->type == NODE_LOGICAL_OPERATOR)*/
/*			break ;*/
/*		ast->is_done = 1;*/
/*		ast = ast->right;*/
/*	}*/
/*}*/
/**/

int	check_syntax_errors(t_token *token)
{
	int error_catched;
	int i;

	error_catched = 0;
	i = 0;
	while (token[i].token_type != TOKEN_EOL)
	{
		if (token[i].token_type == TOKEN_REDIRECT_IN \
			|| token[i].token_type == TOKEN_REDIRECT_OUT \
			|| token[i].token_type == TOKEN_REDIRECT_APPEND)
		{
			check_valid_redir(token, i, &error_catched);
		}
		else if (token[i].token_type == TOKEN_PIPE)
		{
			check_valid_pipe(token, i, &error_catched);
		}
		else if (token[i].token_type == TOKEN_AND \
				|| token[i].token_type == TOKEN_OR)
		{
			check_valid_logical_operator(token, i, &error_catched);
		}
		if (error_catched)
			return (0);
		i++;
	}
	return (1);
}

void	*m_tokenizer(const char *input, const char **env,
			const char *path_variable)
{
	static int	exit_status = 0;
	t_token		*tokens;
	t_ast		*tree;
	int			error_catched;

	error_catched = 0;
	tokens = lexical_analysis(input, env, &exit_status);
	print_tokens(tokens);
	if (!check_syntax_errors(tokens))
		return (NULL);
	tree = parse_tokens(tokens);
	/*if (error_catched)*/
	/*	skip_up_to_logical_operator(tree);*/
	execute_commands(tree, path_variable, env,  &exit_status);
	lst_memory(tokens, NULL, FREE, 0);
	return (NULL);
}
