/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:39:13 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 12:39:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_buildin(t_ast *node)
{
	if (!node->args || !*node->args)
		return (false);
	if (ft_memcmp(node->args[0], "echo", ft_strlen("echo") + 1) == 0
		|| ft_memcmp(node->args[0], "env", ft_strlen("env") + 1) == 0
		|| ft_memcmp(node->args[0], "cd", ft_strlen("cd") + 1) == 0
		|| ft_memcmp(node->args[0], "pwd", ft_strlen("pwd") + 1) == 0
		|| ft_memcmp(node->args[0], "unset", ft_strlen("unset") + 1) == 0
		|| ft_memcmp(node->args[0], "export", ft_strlen("export") + 1) == 0
		|| ft_memcmp(node->args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (true);
	return (false);
}
