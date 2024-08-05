/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:43:20 by anarama           #+#    #+#             */
/*   Updated: 2024/08/05 17:17:36 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_tokens_len(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

char	**copy_args(t_ast *node, char **src)
{
	int		i;
	char	**temp;

	i = get_split_size((const char **)src);
	node->args = ft_calloc(i + 1, sizeof(char *));
	if (!node->args)
	{
		free(node);
		perror("calloc in copy args");
		lst_memory(NULL, NULL, CLEAN, 0);
	}
	lst_memory(node->args, free_split, ADD, 0);
	temp = node->args;
	while (*src)
	{
		*node->args = ft_strdup(*src);
		if (!*node->args)
		{
			free(node);
			perror("strdup in copy args");
			lst_memory(NULL, NULL, CLEAN, 0);
		}
		src++;
		node->args++;
	}
	return (temp);
}
/**/
/*void	append_node(t_ast **head, t_ast *new_node)*/
/*{*/
/*	t_ast	*tmp;*/
/**/
/*	if (!new_node)*/
/*		return ;*/
/*	if (*head)*/
/*	{*/
/*		tmp = *head;*/
/*		while (tmp->right)*/
/*			tmp = tmp->right;*/
/*		new_node->left = tmp;*/
/*		tmp->right = new_node;*/
/*	}*/
/*	else*/
/*		*head = new_node;*/
/*}*/
/**/
/*void	clear_ast(void *head)*/
/*{*/
/*	t_ast	*copy_head;*/
/*	t_ast	*temp;*/
/**/
/*	temp = NULL;*/
/*	copy_head = (t_ast *)head;*/
/*	while (copy_head)*/
/*	{*/
/*		if (copy_head->args)*/
/*			free(copy_head->args);*/
/*		temp = copy_head;*/
/*		copy_head = copy_head->right;*/
/*		free(temp);*/
/*	}*/
/*}*/

int	is_redirection(t_token_type	token_type)
{
	return (token_type == TOKEN_REDIRECT_IN
		|| token_type == TOKEN_REDIRECT_OUT
		|| token_type == TOKEN_REDIRECT_APPEND
		|| token_type == TOKEN_HEREDOC);
}
