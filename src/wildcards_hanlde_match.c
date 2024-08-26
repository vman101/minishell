/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_hanlde_match.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:00:51 by victor            #+#    #+#             */
/*   Updated: 2024/08/26 15:39:58 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	compare_suffix(const char *current_position_in_str,
			const char *current_position_in_pattern)
{
	const char	*suffix;
	size_t		len_current_pattern;
	size_t		len_current_str;

	len_current_pattern = ft_strlen(current_position_in_pattern);
	len_current_str = ft_strlen(current_position_in_str);
	if (len_current_pattern > len_current_str)
		return (-1);
	suffix = current_position_in_str + len_current_str
		- len_current_pattern;
	return (ft_strncmp(current_position_in_pattern, suffix,
			len_current_pattern));
}

int	handle_prefix(const char *pattern, const char *str,
				char *adr_next_wild)
{
	size_t	len_prefix;

	len_prefix = adr_next_wild - pattern;
	if (len_prefix == 0 && str[0] == *(adr_next_wild + 1))
		return (-1);
	return (ft_strncmp(pattern, str, len_prefix));
}

int	handle_suffix(const char *current_position_in_pattern,
				const char	*current_position_in_str)
{
	if (*current_position_in_pattern != '\0')
	{
		if (compare_suffix(current_position_in_str,
				current_position_in_pattern) != 0)
			return (-1);
	}
	return (0);
}

int	handle_middle(const char *pattern, const char *str)
{
	const char	*current_position_in_pattern;
	const char	*current_position_in_str;
	char		*adr_next_wild;
	char		*result;
	int			len_current_pattern;

	adr_next_wild = ft_strchr(pattern, '*');
	current_position_in_pattern = pattern;
	current_position_in_str = str;
	result = 0;
	while (adr_next_wild)
	{
		*((char *)adr_next_wild) = 0;
		len_current_pattern = adr_next_wild - current_position_in_pattern;
		result = ft_strnstr(str, current_position_in_pattern, ft_strlen(str));
		*((char *)adr_next_wild) = '*';
		if (!result)
			return (-1);
		current_position_in_str = result + len_current_pattern;
		current_position_in_pattern = adr_next_wild + 1;
		adr_next_wild = ft_strchr(current_position_in_pattern, '*');
	}
	return (handle_suffix(current_position_in_pattern,
			current_position_in_str));
}
