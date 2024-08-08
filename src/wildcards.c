/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:02:23 by anarama           #+#    #+#             */
/*   Updated: 2024/08/08 13:50:02 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *ptr, int old_size, int new_size)
{
	void *new_ptr;

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
	// free(ptr);
	return (new_ptr);
}

int	get_amount_tokens(t_token *tokens)
{
	int i;

	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		i++;
	}
	return (i);
}

int	compare_suffix(const char *str, const char *ptr)
{
	char	*str_suffix;
	size_t	len_str_suffix;
	size_t	len_ptr;

	len_ptr = ft_strlen(ptr);
	str_suffix = ft_strchr(str, *ptr);
	if (!str_suffix)
		return (-1);
	len_str_suffix = ft_strlen(str_suffix);
	if (len_str_suffix != len_ptr)
		return (-1);
	return (ft_strncmp(ptr, str_suffix, len_str_suffix));
}

int	handle_prefix(const char *pattern, const char *str,
				char *adr_next_wild)
{
	size_t	len_str_prefix;

	len_str_prefix = adr_next_wild - pattern;
	if (str[0] == *(adr_next_wild + 1)) // check for the "*.*""
		return (-1);
	return (ft_strncmp(pattern, str, len_str_prefix));
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

int match_found(const char *pattern, const char *str)
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

char **expand_wildcard(const char *pattern)
{
	struct dirent *entry;
	char **matches;
	int	capacity;
	int	count;
	DIR *dir;
	int flag_match_found;

	matches = NULL;
	flag_match_found = 0;
	capacity = 10;
	count = 0;
	ft_opendir(&dir, ".");
	entry = readdir(dir);
	lst_calloc((void **)&matches, capacity + 1, sizeof(char *));
	while (entry != NULL)
	{
		if (match_found(pattern, entry->d_name) == 0)
		{
			flag_match_found = 1;
			if (count >= capacity)
			{
				matches = (char **)ft_realloc(matches, count * sizeof(char *),
										(count * 2 + 1) * sizeof(char *));
				if (!matches)
				{
					perror("calloc wildcards");
					lst_memory(NULL, NULL, CLEAN);
				}
				lst_memory(matches, free, ADD);
				capacity *= 2;	
			}
			matches[count] = ft_strdup(entry->d_name);
			if (!matches[count])
			{
				perror("strdup wildcards");
				lst_memory(NULL, NULL, CLEAN);
			}
			lst_memory(matches[count], free, ADD);
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (flag_match_found == 0)
		return (NULL);
	return (matches);
}

int	check_wildcard(char *str)
{
	if (!str)
		return (0);
	return (ft_strchr(str, '*') != NULL);
}

void	copy_wildcards(int *k, int *j, char **matches, t_token *new_tokens)
{
	while (matches[*j] != NULL)
	{
		new_tokens[*k].token_value = matches[*j];
		new_tokens[*k].token_type = TOKEN_WORD;
		(*k)++;
		(*j)++;
	}
}

void	copy_old_tokens(int *k, int i, t_token *new_tokens,
					t_token *old_tokens)
{
	new_tokens[*k].token_value = old_tokens[i].token_value;
	new_tokens[*k].token_type = old_tokens[i].token_type;
	(*k)++;
}

void	copy_tokens_with_wildcards(t_token *new_tokens,
					t_token *old_tokens, char **matches)
{
	int flag;
	int i;
	int j;
	int k;

	flag = 1;
	i = 0;
	j = 0;
	k = 0;
	while (old_tokens[i].token_type != TOKEN_EOL)
	{
		if (flag == 1 && check_wildcard(old_tokens[i].token_value))
		{
			copy_wildcards(&k, &j, matches, new_tokens);
			flag = 0;
		}
		else
		{
			copy_old_tokens(&k, i, new_tokens, old_tokens);
		}
		i++;
	}
	new_tokens[k].token_value = NULL;
	new_tokens[k].token_type = TOKEN_EOL;
}

int	handle_wildcard(t_token **tokens, t_token **new_tokens,
				int i, int *size)
{
	char	**matches;
	int		match_count;
	int		match_found;

	matches = expand_wildcard((*tokens)[i].token_value);
	match_found = 0;
	if (matches)
	{
		match_found = 1;
		match_count = get_tokens_count(matches);
		if (*new_tokens != NULL) // to avoid adding last realloc to lst
			lst_memory(*new_tokens, free, ADD);
		*new_tokens = ft_realloc(*new_tokens, *size * sizeof(t_token),
						(*size + match_count) * sizeof(t_token));
		*size = *size + match_count - 1;
		copy_tokens_with_wildcards(*new_tokens, *tokens, matches);
		*tokens = *new_tokens;
	}
	return (match_found);
}

void check_and_expand_wildcards(t_token	**tokens_ptr)
{
	t_token	*tokens;
	t_token *new_tokens;
	int		match_found;
	int		size;
	int		i;

	match_found = 0;
	tokens = *tokens_ptr;
	size = get_amount_tokens(tokens);
	new_tokens = NULL;
	i = 0;
	while (tokens[i].token_type != TOKEN_EOL)
	{
		if (check_wildcard(tokens[i].token_value))
		{
			match_found += handle_wildcard(&tokens, &new_tokens, i, &size);
		}
		i++;
	}
	if (match_found)
	{
		lst_memory(*tokens_ptr, NULL, FREE);
		*tokens_ptr = tokens;
		lst_memory(*tokens_ptr, free_tokens, ADD);	
	}
}

// REFACTOR THE CODE 
// IMPR INPUT CHECK WITH QUOTES