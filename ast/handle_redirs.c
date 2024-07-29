/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:56:47 by anarama           #+#    #+#             */
/*   Updated: 2024/07/29 16:11:10 by anarama          ###   ########.fr       */
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

void	handle_redir_out(t_ast *save_ptr_left, t_ast *redir_node)
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
		save_ptr_left->error_found = 1;
		redir_node->error_found = 1;
		return ;
	}
	save_ptr_left->file = redir_node->file;
	if (redir_node->right && redir_node->right->args && !redir_node->right->is_done)
	{
		save_ptr_left->args = cat_args(save_ptr_left->args, redir_node->right->args);
		redir_node->right->is_done = 1;
	}
}

void	handle_redir_in(t_ast *save_ptr_left, t_ast *redir_node)
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
		save_ptr_left->error_found = 1;
		redir_node->error_found = 1;
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
	if (redir_node->left == NULL)
	{
		t_ast *temp = create_command_node(TOKEN_WORD, NULL);
		lst_memory(temp, free, ADD);
		temp->right = redir_node;
		redir_node->left = temp;
		*head = temp;
	}
	else if (redir_node->left->type == NODE_LOGICAL_OPERATOR)
	{
		t_ast *temp = create_command_node(TOKEN_WORD, NULL);
		temp->right = redir_node;
		temp->left = redir_node->left;
		if (temp->left)
		{
			redir_node->left->right = temp;
		}
		redir_node->left = temp;
	}
	else
    {
        t_ast *current = redir_node;
        while (current->left && current->left->type != NODE_LOGICAL_OPERATOR)
        {
            if (current->left->type == NODE_COMMAND && !current->left->is_done)
                break;
            current = current->left;
        }
    }
}

void	handle_redir(t_ast *redir_node, t_ast **head)
{
	t_ast	*save_ptr_left;

	setup_left_command_node(redir_node, head);
	save_ptr_left = redir_node->left;
	save_ptr_left->right = redir_node;
	while (redir_node && redir_node->type == NODE_REDIRECTION)
	{
		setup_flags_and_fds(redir_node, save_ptr_left);
		if (redir_node->token_type == TOKEN_REDIRECT_IN 
			|| redir_node->token_type == TOKEN_REDIRECT_APPEND)
		{
			handle_redir_in(save_ptr_left, redir_node);
		}
		else if (redir_node->token_type == TOKEN_REDIRECT_OUT)
		{
			handle_redir_out(save_ptr_left, redir_node);
		}
		redir_node->is_done = 1;
		redir_node = redir_node->right;
		if (save_ptr_left->error_found)
		{
			return ;
		}
	}
}
