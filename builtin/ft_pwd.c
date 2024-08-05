/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:49:03 by victor            #+#    #+#             */
/*   Updated: 2024/08/05 16:48:56 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int32_t	ft_pwd(const char **environment)
{
	char	*pwd;

	pwd = environment_variable_value_get("PWD", environment);
	ft_putendl_fd(pwd, 1);
	return (0);
}
