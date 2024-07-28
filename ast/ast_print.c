/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrejarama <andrejarama@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:43:48 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 22:10:18 by andrejarama      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_ast(t_ast *head)
{
	int	i;
	int j;
	char **args;

	i = 0;
	j = 0;
	printf("----AST----\n");
	while (head)
	{
		if (!head->is_done)
		{
			printf("NODE %d\n", i);
			if (head->type == NODE_COMMAND)
				printf("TYPE: COMMAND\n");
			else if (head->type == NODE_REDIRECTION)
				printf("TYPE: REDIRECTION\n");
			else if (head->type == NODE_PIPE)
				printf("TYPE: PIPE\n");
			else if (head->type == NODE_LOGICAL_OPERATOR)
				printf("TYPE: LOGICAL_OPERATOR\n");
			if (head->token_type == TOKEN_REDIRECT_IN)
				printf("TOKEN TYPE: REDIRECT_IN\n");
			else if (head->token_type == TOKEN_REDIRECT_OUT)
				printf("TOKEN TYPE: REDIRECT_OUT\n");
			else if (head->token_type == TOKEN_REDIRECT_APPEND)
				printf("TOKEN TYPE: REDIRECT_APPEND\n");
			else if (head->token_type == TOKEN_HEREDOC)
				printf("TOKEN TYPE: HEREDOC\n");
			else if (head->token_type == TOKEN_AND)
				printf("TOKEN TYPE: AND\n");
			else if (head->token_type == TOKEN_OR)
				printf("TOKEN TYPE: OR\n");
			printf("ARGS: ");
			args = head->args;
			if (args && *args)
			{
				while (*args)
				{
					printf("[%s]", *args);
					args++;
				}
			}
			else
			{
				printf("NULL");
			}
			printf("\n");
			printf("LEFT: %p\n", head->left);
			printf("RIGHT: %p\n", head->right);
			printf("FILE: %s\n", head->file);
			printf("FD_FILE_IN: %d\n", head->fd_file_in);
			printf("FD_FILE_OUT: %d\n", head->fd_file_out);
			printf("FD_IN: %d\n", head->fd_in);
			printf("FD_OUT: %d\n", head->fd_out);
			printf("STD_FD: %d\n", head->std_fd);
			printf("FLAGS: %d\n", head->flags);
			printf("ERROR_FOUND: %d\n", head->error_found);
			printf("IS_DONE: %d\n", head->is_done);
			i++;
			printf("\n");
		}
		head = head->right;
	}
	printf("-----------\n");
}
