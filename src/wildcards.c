/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:02:23 by anarama           #+#    #+#             */
/*   Updated: 2024/08/23 11:02:09 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	char	**new_args;
	int		match_found;
	int		size;
	int		i;

	if (!*input)
		return ;
	match_found = 0;
	args = *input;
	size = get_split_size((const char **)*input);
	new_args = NULL;
	i = 0;
	while (args[i] != NULL)
	{
		if (check_wildcard(args[i]))
			match_found += handle_wildcard(&args, &new_args, i, &size);
		i++;
	}
	if (match_found)
	{
		free(*input);
		*input = args;
	}
}
