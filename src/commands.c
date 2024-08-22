/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 21:20:49 by victor            #+#    #+#             */
/*   Updated: 2024/08/22 18:19:01 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_fd(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
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
		/*print_tokens(tokens);*/
		tree = parse_tokens(tokens, exit_status);
		if (tree)
			execute_commands(tree, env, exit_status);
		lst_memory(tokens, NULL, FREE);
	}
	return (NULL);
}
