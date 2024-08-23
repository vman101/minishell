/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvobis <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 23:43:36 by vvobis            #+#    #+#             */
/*   Updated: 2024/08/23 15:52:33 by vvobis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H

# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include "ft_printf.h"
# include <stdbool.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* Input Handeling */
char	***super_split(char **strs, int size, int delimiter);
void	*free_super_split(char ***back);
char	*get_next_line(int fd);

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isprint(int c);
int		ft_isascii(int c);
int		is_space(char const c);

/*Memory Management*/
int		ft_memcmp(void *s1, void const *s2, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, void const *src, size_t n);
void	*ft_memmove(void *dest, void const *src, size_t n);
void	*ft_memchr(void *s, int c, size_t n);
void	*ft_calloc(size_t n, size_t s);

/*Info Conversion*/
int		ft_atoi(char const *s);
long	ft_atol(char const *s, uint8_t *too_big);
char	*ft_itoa(int n);
char	*ft_ltoa(long long n);
char	**ft_split(char const *s, char c);

/*String Manip*/
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(char const *s1, char const *s2, size_t n);
char	*ft_strnstr(char const *s1, char const *s2, size_t n);
char	*ft_strchr(char const *s, int c);
char	*ft_strrchr(char const *s, int c);
char	*ft_strdup(char const *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char const *s, void (*f)(unsigned int, char *));
size_t	ft_strlcpy(char *dest, char const *src, size_t size);
size_t	ft_strlcat(char *dest, char const *src, size_t size);
size_t	ft_strlen(char const *str);

/*List manip*/
int		ft_lstsize(t_list *lst);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstadd_front(t_list **lst, t_list *node_new);
void	ft_lstadd_back(t_list **lst, t_list *node_new);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);

/*Output*/
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(const char *str, int fd);
void	ft_putendl_fd(char *str, int fd);
void	ft_putnbr_fd(int n, int fd);

/* safe functions */

void	ft_free(void *ptr);

#endif
