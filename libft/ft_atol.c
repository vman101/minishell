/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:49:04 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/26 21:00:15 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(char const *s, uint8_t	*too_big)
{
	unsigned long long	nb;
	char const			*tmp;

	nb = 0;
	while (is_space(*s))
		s++;
	tmp = s;
	if (*tmp == '+' || *tmp == '-')
		tmp++;
	while (*tmp >= '0' && *tmp <= '9')
	{
		nb *= 10;
		nb += (*tmp - '0');
		tmp++;
		if (nb > (uint64_t)LLONG_MAX + (*s == '-'))
			return (*too_big == 1);
	}
	if (*s == '-')
		nb = -nb;
	return (nb);
}
