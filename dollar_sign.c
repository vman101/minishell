/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 13:48:27 by anarama           #+#    #+#             */
/*   Updated: 2024/07/17 18:00:12 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	custom_memmove_chars(char *dest, char *src, int flag_add_equel_sign)
{
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	if (flag_add_equel_sign)
	{
		*dest = '=';
		dest++;
	}
	while (*dest)
	{
		*dest = '\0';
		dest++;
	}
}

// if var wasnt found i still return an original token but with = on end, need to fix it
// if token len is 1 and token is ? i need to handle it seperatelly
void	handle_dollar_sign(char **single_token, char **env)
{
	char	*temp;

	temp = NULL;
	custom_memmove_chars(*single_token, *single_token + 1, 1);
	while (*env)
	{
		if (ft_strncmp(*single_token, *env, ft_strlen(*single_token)) == 0)
		{
			temp = ft_strdup(*env);
			break;
		}
		env++;
	}
	if (temp == NULL)
	{
		free(*single_token);
		*single_token = NULL;
		return ;
	}
	free(*single_token);
	*single_token = temp;
	custom_memmove_chars(*single_token, *single_token + 5, 0);
	temp = NULL;
}
