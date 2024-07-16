/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:16:38 by victor            #+#    #+#             */
/*   Updated: 2024/07/16 14:46:59 by vvobis           ###   ########.fr       */
/*   Updated: 2024/07/07 20:14:06 by anarama          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdio.h>
# include <dirent.h>
# include <string.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define PROMPT_COMMAND_STACK_SIZE 6
# define PROMPT_INPUT_BUFFER_SIZE 1024
# define CURSOR_MOVE_HOME "\033[H"
# define GREEN "\033[0;32m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
# define CURSOR_MOVE_UP "\033[A"
# define CURSOR_MOVE_DOWN "\033[B"
# define CURSOR_MOVE_RIGHT "\033[C"
# define CURSOR_MOVE_LEFT "\033[D"
# define CURSOR_POSITION_GET "\033[6n"
# define CURSOR_POSITION_SET "\033[%d;%dH"
# define CURSOR_POSITION_SAVE "\033[s"
# define CURSOR_POSITION_RESTORE "\033[u"

# define MAXIMUM_COLUMN 9999
# define MAXIMUM_ROW 9999
# define SCREEN_MAX "\033[9999;9999H"
# define DEL 127
# define EOT 4

# define SCREEN_DISBLE_WRAPPING "\033[?7l"
# define SCREEN_ENABLE_WRAPPING "\033[?7h"
# define SCREEN_CLEAR_TO_EOL "\033[K"
# define SCREEN_CLEAR "\033[2J"
# define SCREEN_CLEAR_TO_EOF "\033[J"

typedef struct s_prompt
{
	char		*command;
	uint32_t	prompt_length;
	uint32_t	history_position_current;
	uint32_t	history_count;
	char		**history_entries;
	char		**env_ptr;
}				t_prompt;

enum e_alloc
{
	ADD,
	CLEAN,
	END,
	FREE,
};

typedef struct s_clean
{
	void			*content;
	void			(*clean)(void *del);
	struct s_clean	*next;
}					t_clean;

extern int32_t g_signal_flag;

/* Builtins */
void		ft_chdir(const char *path);
void		ft_exit();

/* Commands */
bool		buildin_execute(char const *command, char const **argv);
void 		command_execute(char const *command_path, char const *argv[], char const **env);
void		*m_tokenizer(const char *input, const char **env, const char *path_variable);

/* Handle signal */
void		handle_sigint(int sig);
void		setup_signal_handlers();

/* Input */
void		free_split(void *back);

/* List Memory */
void		lst_memory(void *mem, void (*del)(void *c), int mode);
int			p_stderr(int stderr_fd, const char *error, const char *specifier);

/* List */
t_clean	*	lst_node_new(void *content, void (*del)(void *));
void		lst_node_del(t_clean **lst);
void		lst_node_del_clean(t_clean **lst, void *mem);
void		lst_list_clean(t_clean **head);
int			lst_add_back(t_clean **node, t_clean *node_new);

/* Path Utils */
char		*find_absolute_path(const char *path_variable, char *input);

/* Prompt */
void		prompt_destroy(void *prompt);
uint32_t	prompt_display();
t_prompt	*prompt_create(const char **env);
char		*prompt_get(t_prompt *prompt);

/* Cursor Manipulation */
void		cursor_position_get(uint32_t cursor_position[2]);
void		cursor_position_save(void);
void		cursor_position_restore(void);

uint8_t		handle_escape_sequence(t_prompt *prompt, char	**input, uint32_t *cursor_position_current);
char		*prompt_get_input(t_prompt *prompt);

/* Prompt Buffer Management */
void		prompt_refresh_line(char *input, uint32_t cursor_position_current[2]);
void		prompt_buffer_size_manage(char **input, uint32_t input_new_size);
void		prompt_string_insert(char *string_to_insert, char **current_input, char *position_to_insert, uint32_t current_word_length);

/* Redirections */
void		check_redirections(char **tokens, char **env);

/* Tab Completion */
uint32_t		handle_tab(char **input, const char **env, uint32_t *cursor_position_current);

/* Termios */
void		terminal_raw_mode_enable();
void 		terminal_raw_mode_disable();

/* Utils */
int			ft_close(int fd, const char *specifier);
void		ft_pipe(int pipefd[2], const char *specifier);
void		ft_dup2(int fd_old, int fd_new, const char *specifier);
void		ft_fork(pid_t *pid, const char *specifier);
void		ft_open(int *fd, const char *path, int flag, int mode);
int64_t		ft_read(int fd, char *character, char **input, uint32_t size_to_read);
void		ft_opendir(DIR **directory, const char *directory_path);
void		print_debug_uint(uint32_t info);
void		print_debug_string(char *string);
void		error_log(char *specifier, void *info);

/* Utils2 */
void		custom_memmove_strings(char **dest, char **src);
void		print_tokens(char **tokens);
int			get_tokens_count(char **tokens);
void		custom_free_split(char **arr);
void		handle_dollar_sign(char **single_token, char **env);
void		ft_echo(char **tokens, int fd);
void		ft_pwd(int fd, char **env);
void		ft_env(char **env, int fd);
uint32_t	get_split_size(const char **environment);

/* Environment_variable */

void		environment_variable_print(const char *variable, const char **environment);
void		environment_print(char **environment);
char		**environment_create(const char **env);
char		**environment_variable_add(char **environment, const char *variable_new_name, const char *variable_new_value);

#endif
