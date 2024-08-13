/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:11:25 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/02 16:12:18 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prompt_print_pwd(char *prompt)
{
	ft_putstr_fd(GREEN, 1);
	ft_printf("[%s]$ ", prompt);
	ft_putstr_fd(RESET, 1);
}

void	prompt_print_custom_string(char *string)
{
	ft_putstr_fd(string, 1);
}
