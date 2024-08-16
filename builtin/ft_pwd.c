/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:49:03 by victor            #+#    #+#             */
/*   Updated: 2024/08/16 23:50:58 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(const char **environment, int32_t *exit_status)
{
	char	*pwd;

	if (!environment)
		*exit_status = 1;
	else
	{
		*exit_status = 0;
		pwd = getcwd(NULL, 0);
		ft_putendl_fd(pwd, 1);
		ft_free(&pwd);
	}
}
