/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_special_symbol.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:44:00 by anarama           #+#    #+#             */
/*   Updated: 2024/07/26 15:49:11 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_special_char(char c)
{
	return (c == ' ' || c == '|' || c == '>'
		|| c == '<' || c == '\'' || c == '\"');
}

int	ft_isspace(char c)
{
	return (c == ' ');
}

int	ft_is_single_quote(char c, int *second_quote_found)
{
	if (c == '\'')
	{
		*second_quote_found = 1;
	}
	return (c == '\'');
}

int	ft_is_double_quote(char c, int *second_double_found)
{
	if (c == '\"' && second_double_found)
	{
		*second_double_found = 1;
	}
	return (c == '\"');
}
