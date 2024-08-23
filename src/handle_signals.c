/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:28:41 by anarama           #+#    #+#             */
/*   Updated: 2024/08/23 17:05:02 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void unset_sigquit_handler(void)
{
	struct sigaction sa;
	
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_flag = 1;
	}
	else if (sig == SIGQUIT)
	{
		g_signal_flag = 3;
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	bzero(&sa, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	/*sa.sa_handler = SIG_IGN;*/
	sigaction(SIGTSTP, &sa, NULL);
}
