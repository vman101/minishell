/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_special_symbol.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:44:00 by anarama           #+#    #+#             */
/*   Updated: 2024/08/21 10:51:22 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_char(char c)
{
	return (c == ' ' || c == '|' || c == '>' || c == '$'
		|| c == '<' || c == '&' || c == ';');
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v'
		|| c == '\f' || c == '\r' || c == '\n');
}

bool	unrecognized_input(char c)
{
	if (c >= 0 && c < 127 && (ft_isalnum(c) \
		|| ft_isspace(c) || is_special_char(c) \
		|| c == '?' || c == '/' || c == '-' || c == '\n' \
		|| c == ';' || c == '\'' || c == '\"' || c == '.'\
		|| c == '(' || c == ')' || c == '*' || c == '[' \
		|| c == ']' || c == '{' || c == '}' || c == '#'))
		return (false);
	p_stderr(2, "minishell: syntax error near unexpected token `%s'\n", \
			(char []){c, 0});
	return (true);
}

bool	is_in_alphabet(char c)
{
	return (is_special_char(c) \
		|| c == '/' || c == '?' \
		|| c == '-' || c == '\n' \
		|| c == ';' || c == '\'' \
		|| c == '\"' || c == '*' || c == '.');
}
