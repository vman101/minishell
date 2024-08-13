/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:02:23 by anarama           #+#    #+#             */
/*   Updated: 2024/08/13 15:03:34 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *ptr, int old_size, int new_size)
{
	void	*new_ptr;

	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
	{
		new_ptr = ft_calloc(new_size, 1);
		return (new_ptr);
	}
	new_ptr = ft_calloc(new_size, 1);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	return (new_ptr);
}

int	get_amount_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

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
	if (str[0] == *(adr_next_wild + 1))
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

int	match_found(const char *pattern, const char *str)
{
	char	*adr_next_wild;
	int		len_prefix;

	if (*pattern == '*' && *(pattern + 1) == '\0')
		return (0);
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

void	copy_wildcards(int *k, int *j, char **matches,
				char **new_tokens)
{
	while (matches[*j] != NULL)
	{
		new_tokens[*k] = matches[*j];
		(*k)++;
		(*j)++;
	}
}

void	copy_old_tokens(int *k, int i, char **new_tokens,
					char **old_tokens)
{
	new_tokens[*k] = old_tokens[i];
	(*k)++;
}

void	copy_tokens_with_wildcards(char **new_args,
					char **old_args, char **matches)
{
	int	flag;
	int	i;
	int	j;
	int	k;

	flag = 1;
	i = 0;
	j = 0;
	k = 0;
	while (old_args[i] != NULL)
	{
		if (flag == 1 && check_wildcard(old_args[i]))
		{
			copy_wildcards(&k, &j, matches, new_args);
			flag = 0;
		}
		else
		{
			copy_old_tokens(&k, i, new_args, old_args);
		}
		i++;
	}
	new_args[k] = NULL;
}

int	handle_wildcard(char ***args, char ***new_args,
				int i, int *size)
{
	char	**matches;
	int		match_count;
	int		match_found;

	matches = expand_wildcard((*args)[i]);
	match_found = 0;
	if (matches)
	{
		match_found = 1;
		match_count = get_tokens_count(matches);
		if (*new_args != NULL)
			lst_memory(*new_args, free, ADD);
		*new_args = ft_realloc(*new_args, *size * sizeof(char *),
				(*size + match_count) * sizeof(char *));
		*size = *size + match_count - 1;
		copy_tokens_with_wildcards(*new_args, *args, matches);
		*args = *new_args;
	}
	return (match_found);
}

void	check_and_expand_wildcards(char ***input)
{
	char	**args;
	char 	**new_args;
	int		match_found;
	int		size;
	int		i;

	match_found = 0;
	args = *input;
	size = get_split_size((const char **)*input);
	new_args = NULL;
	i = 0;
	while (args[i] != NULL)
	{
		if (check_wildcard(args[i]))
		{
			match_found += handle_wildcard(&args, &new_args, i, &size);
		}
		i++;
	}
	if (match_found)
	{
		free(*input);
		*input = args;
		//lst_memory(*input, free_split, ADD);
	}
}

// IMPR INPUT CHECK WITH QUOTES