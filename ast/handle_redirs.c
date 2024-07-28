/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/28 14:46:51 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**cat_args(char **left, char **right)
{
	char	**new_arr;
	int		len_left;
	int		len_right;
	int		i;

	len_left = get_split_size((const char **)left);
	len_right = get_split_size((const char **)right);
	new_arr = ft_calloc(len_left + len_right + 1, sizeof(t_token *));
	lst_memory(new_arr, free_split, ADD);
	i = 0;
	while (i < len_left)
	{
		new_arr[i] = ft_strdup(left[i]);
		i++;
	}
	i = 0;
	while (i < len_right)
	{
		new_arr[i + len_left] = ft_strdup(right[i]);
		i++;
	}
	return (new_arr);
}

void	check_valid_redir(t_ast *redir_node, int *error_catched)
{
	if (!redir_node->file)
	{
		printf("minishell: syntax error near unexpected token 'newline'\n");
		*error_catched = 1;
	}
	else if (is_double_special(redir_node->file) || is_single_special(redir_node->file))
	{
		printf("minishell: syntax error near unexpected token '%s'\n", redir_node->file);
		*error_catched = 1;
	}
}

void	setup_flags_and_fds(t_ast *redir_node, t_ast *command_node)
{
	if (redir_node->token_type == TOKEN_REDIRECT_IN)
	{
		command_node->flags = O_WRONLY | O_CREAT | O_TRUNC;
		command_node->std_fd = STDOUT_FILENO;
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
	{
		command_node->flags = O_RDONLY;
		command_node->std_fd = STDIN_FILENO;
	}
	else if (redir_node->token_type == TOKEN_REDIRECT_APPEND)
	{
		command_node->flags = O_WRONLY | O_CREAT | O_APPEND;
		command_node->std_fd = STDOUT_FILENO;
	}
}

void	handle_redir_out(t_ast *save_ptr_left, t_ast *redir_node, int *error_catched)
{
	if (save_ptr_left->fd_file_in)
	{
		ft_close(save_ptr_left->fd_file_in, "redir");
		save_ptr_left->fd_file_in = 0;
	}
	ft_open(&save_ptr_left->fd_file_in, redir_node->file,
		save_ptr_left->flags, 0644);
	if (save_ptr_left->fd_file_in == -1)
	{
		*error_catched = 1;
		return ;
	}
	save_ptr_left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args && !redir_node->right->is_done)
	{
		save_ptr_left->args = cat_args(save_ptr_left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
}

void	handle_redir_in(t_ast *save_ptr_left, t_ast *redir_node, int *error_catched)
{
	if (save_ptr_left->fd_file_out)
	{
		ft_close(save_ptr_left->fd_file_out, "redir");
		save_ptr_left->fd_file_out = 0;
	}
	ft_open(&save_ptr_left->fd_file_out, redir_node->file,
		save_ptr_left->flags, 0644);
	if (save_ptr_left->fd_file_out == -1)
	{
		*error_catched = 1;
		return ;
	}
	save_ptr_left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args && !redir_node->right->is_done)
	{
		save_ptr_left->args = cat_args(save_ptr_left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
}

void	setup_left_command_node(t_ast *redir_node, t_ast **head)
{
	if (redir_node->left == NULL || redir_node->left->type == NODE_LOGICAL_OPERATOR)
	{
		t_ast *temp = create_command_node(TOKEN_WORD, NULL);
		lst_memory(temp, free, ADD);
		temp->right = redir_node;
		redir_node->left = temp;
		*head = temp;
	}
	while (redir_node->left && redir_node->left->type != NODE_LOGICAL_OPERATOR)
	{
		if (redir_node->left->type == NODE_COMMAND && !redir_node->left->is_done)
			break ;
		redir_node->left = redir_node->left->left;
	}
}

void	handle_redir(t_ast *redir_node, t_ast **head, int *error_catched)
{
	t_ast	*save_ptr_left;

	check_valid_redir(redir_node, error_catched);
	if (*error_catched)
		return ;
	setup_left_command_node(redir_node, head);
	save_ptr_left = redir_node->left;
	save_ptr_left->right = redir_node;
	while (redir_node && redir_node->type == NODE_REDIRECTION)
	{
		setup_flags_and_fds(redir_node, save_ptr_left);
		if (redir_node->token_type == TOKEN_REDIRECT_IN 
			|| redir_node->token_type == TOKEN_REDIRECT_APPEND)
		{
			handle_redir_in(save_ptr_left, redir_node, error_catched);
		}
		else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
		{
			handle_redir_out(save_ptr_left, redir_node, error_catched);
		}
		redir_node->is_done = 1;
		redir_node = redir_node->right;
		if (*error_catched)
			return ;
	}
}
