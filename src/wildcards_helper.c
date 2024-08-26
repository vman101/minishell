/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victor </var/spool/mail/victor>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:04:15 by victor            #+#    #+#             */
/*   Updated: 2024/08/26 15:45:44 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	match_found(const char *pattern, const char *str)
{
	char	*adr_next_wild;
	int		len_prefix;

	if (*pattern == '*' && *(pattern + 1) == '\0')
	{
		if (str[0] == '.')
			return (1);
		return (0);
	}
	adr_next_wild = ft_strchr(pattern, '*');
	if (adr_next_wild)
	{
		if (handle_prefix(pattern, str, adr_next_wild) != 0)
			return (-1);
		len_prefix = adr_next_wild - pattern;
		pattern += len_prefix + 1;
		str += len_prefix;
	}
	return (handle_middle(pattern, str));
}

void	lst_calloc(void **ptr, int num, int size)
{
	*ptr = ft_calloc(num, size);
	if (!*ptr)
	{
		perror("calloc wildcards");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory(*ptr, free, ADD);
}

void	add_new_match(int *count, int *capacity,
			char ***matches, char *entry_name)
{
	if (*count >= *capacity)
	{
		*matches = (char **)ft_realloc(*matches, *count * sizeof(char *),
				(*count * 2 + 1) * sizeof(char *));
		if (!*matches)
		{
			perror("calloc wildcards");
			lst_memory(NULL, NULL, CLEAN);
		}
		lst_memory(*matches, free, ADD);
		*capacity *= 2;
	}
	(*matches)[*count] = ft_strdup(entry_name);
	if (!(*matches)[*count])
	{
		perror("strdup wildcards");
		lst_memory(NULL, NULL, CLEAN);
	}
	lst_memory((*matches)[*count], free, ADD);
	(*count)++;
}

char	**expand_wildcard(const char *pattern)
{
	struct dirent	*entry;
	char			**matches;
	DIR				*dir;
	int				capacity;
	int				count;

	capacity = 10;
	matches = NULL;
	count = 0;
	ft_opendir(&dir, ".");
	entry = readdir(dir);
	lst_calloc((void **)&matches, capacity + 1, sizeof(char *));
	while (entry != NULL)
	{
		if (match_found(pattern, entry->d_name) == 0)
		{
			add_new_match(&count, &capacity,
				&matches, entry->d_name);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (count == 0)
		return (NULL);
	return (matches);
}

int	check_wildcard(char *str)
{
	if (!str)
		return (0);
	return (ft_strchr(str, '*') != NULL);
}
