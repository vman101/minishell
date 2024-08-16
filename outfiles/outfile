/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:16:38 by victor            #+#    #+#             */
/*   Updated: 2024/08/16 22:25:45 by victor           ###   ########.fr       */
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
# include <errno.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <stddef.h>

# define PWD 0
# define CUSTOM 1

# define PROMPT_COMMAND_STACK_SIZE 6
# define PROMPT_INPUT_BUFFER_SIZE 4096
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
# define ESC 27

# define VARIABLE_TOKEN_SIZE 1024

# define SCREEN_DISBLE_WRAPPING "\033[?7l"
# define SCREEN_ENABLE_WRAPPING "\033[?7h"
# define SCREEN_CLEAR_TO_EOL "\033[K"
# define SCREEN_CLEAR "\033[2J"
# define SCREEN_CLEAR_TO_EOF "\033[J"

# define ENVIRONMENT_SIZE 512
# define INITIAL_TOKEN_CAPACITY 16

# define UNIMPLEMENTED(descriptor) do { ft_printf("%s unimplemnted: file: %s line %d\n", descriptor, __FILE__, __LINE__); } while (0)

typedef struct s_prompt
{
	bool		exists;
	char		*prompt;
	uint32_t	cursor_position[2];
	void		(*prompt_display_func)(char *);
	uint32_t	prompt_length;
	uint32_t	history_position_current;
	uint32_t	history_count;
	char		*command;
	char		**history_entries;
	char		**env_ptr;
}				t_prompt;

typedef enum
{
	TOKEN_WORD,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NEWLINE,
	TOKEN_EOL,
    TOKEN_ENV,
    TOKEN_EXIT_STATUS,
    TOKEN_DONE,
} t_token_type;

typedef enum
{
	TREE_NONE = 0,
	TREE_PIPE = 7,
	TREE_LOGICAL_AND,
	TREE_LOGICAL_OR,
	TREE_NEWLINE,
	TREE_INVALID
} t_tree_connection_type;

typedef struct s_token
{
	t_token_type	token_type;
	char			*token_value;
}	t_token;

typedef enum
{
	NODE_INVALID = 1,
	NODE_END,
    NODE_COMMAND,
    NODE_REDIRECTION,
    NODE_PIPE,
    NODE_LOGICAL_OPERATOR,
} t_node_type;

typedef enum e_file_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND
}	t_file_type;

typedef struct s_ast
{
    t_node_type		type;
	t_token_type	token_type;
	t_tree_connection_type connection_type;
    char 			**args;
	char			*variable_pointers;
	char			*path;
	bool			has_redir_in;
	bool			has_redir_out;
	int32_t			pipefd[2];
    int				fd_in;
	int				fd_out;
	int				flags;
	int				is_done;
} t_ast;


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

extern volatile int32_t g_signal_flag;

void set_non_blocking();
/* Builtins */
int32_t		ft_echo(char **args, int32_t *exit_status);
void		ft_cd(const char **environment, const char **args, int32_t *exit_status);
void		ft_pwd(const char **env, int32_t *exit_status);
void		ft_env(const char **env, int32_t *exit_status);
void		ft_unset(char **environment, const char **args, int32_t *exit_status);
void		ft_export(const char **args, int32_t *exit_status);
void		ft_exit(const char **args);

/* Commands */
bool		buildin_execute(t_ast *node, const char **environment, int *exit_status);
void 		command_execute(char const *command_path, char const *argv[], char const **env);
void		*m_tokenizer(const char *input, const char **env, int *exit_status);
/* Handle signal */
void		handle_sigint(int sig);
void		setup_signal_handlers();

/* Input */
void		free_split(void *back);
uint32_t    get_split_size(const char **split);
uint32_t    get_split_length(char **split);

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
t_prompt	prompt_create(const char **env, uint8_t mode);
char		*prompt_get(const char **environment);
uint32_t	prompt_display_string_set(t_prompt *prompt, const char **environment, const char *prompt_string);
void	handle_arrow_key_up(	t_prompt *prompt,
									char **input,
									uint32_t cursor_position_current[2]);

/* Prompt_print.c */
void	prompt_print_custom_string(char *string);
void	prompt_print_pwd(char *prompt);

/* Lexer */
void	evaluate_input(char **input[], int32_t *exit_status, bool *error_caught);

/* Cursor Manipulation */
void		cursor_position_get(uint32_t cursor_position[2]);
void		cursor_position_save(void);
void		cursor_position_restore(void);

bool		handle_escape_sequence(t_prompt *prompt, char buffer[], char **input, uint32_t cursor_position_current[2]);
char		*prompt_get_input(t_prompt *prompt, uint32_t prompt_initial_size, const char *delimiter);
bool		handle_multiple_character_to_input(	char **input, char buffer[], uint32_t cursor_position_current[2], uint32_t prompt_length_current);

/* Prompt Buffer Management */
void		prompt_refresh_line(char *input, uint32_t cursor_position_base, uint32_t cursor_position_current[2]);
char		*prompt_buffer_size_manage(char **input, uint32_t old_size, uint32_t size_to_add);
void		prompt_string_insert(char *string_to_insert, char **current_input, char *position_to_insert, uint32_t current_word_length);
bool		handle_new_character_to_input(		char **input, char character, uint32_t *cursor_position_current, uint32_t prompt_length_current);
void		blocking_mode_toggle(int flag);
uint8_t		handle_single_char_input(	char **input, char buffer[], \
										uint32_t cursor_position_current[2], \
										bool *do_refresh);
uint8_t		handle_single_char_input(	char **input, char buffer[], \
										uint32_t cursor_position_current[2], \
										bool *do_refresh);
bool		handle_new_character_to_input(	char **input, \
											char character, \
											uint32_t *cursor_position_current, \
											uint32_t prompt_length_current);
/* Non Blocking mode */

/* Redirections */
void		execute(char **tokens, char **env);
/* Tab Completion */
void		handle_tab(char **input, t_prompt *prompt);
void		handle_tab_no_match(const char *input_path, \
								char **env, \
								uint32_t cursor_position_current[2], \
								t_prompt *prompt);
void		handle_tab_yes_match(	t_prompt *prompt, \
									const char *next_word_match, \
									char **input, \
									uint32_t current_word_length);
char		*determine_word(char *input, \
							char **input_path, \
							uint32_t cursor_position_current);
void		get_next_word_match(	char **input, \
									t_prompt *prompt, \
									char *input_path, \
									bool *is_directory);

void		handle_rapid_input(	char buffer[], \
								uint32_t cursor_position[2], \
								char *input, \
								uint32_t cursor_position_base);
void	handle_backspace(	char *input, \
							uint32_t *cursor_position_current, \
							uint32_t input_length_current);
/* Termios */
void		terminal_raw_mode_enable(int);
void 		terminal_raw_mode_disable(int);

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
int			get_tokens_count(char **tokens);
void		custom_free_split(char **arr);
void		handle_dollar_sign(char **single_token, char **env);

/* Wildcards */
void		check_and_expand_wildcards(char	***input);
int			check_wildcard(char *str);

/* Environment_variable */

void		environment_variable_print(const char *variable, const char **environment);
void		environment_print(const char **environment);
char		**environment_create(const char **env);
void		environment_variable_remove(char **environment, const char *variable);
void		environment_variable_add(char ***environment, const char *variable_new_name, const char *variable_new_value);
char		**environment_variable_get(const char *variable, const char **environment);
char		*environment_variable_value_get(const char *variable, const char **environment);
void		environment_variable_value_change(const char **environment, const char *variable_name, const char *variable_new_value);
char		**env_static(char **environment);

/* TOKENIZER MOTHERFUCKER!!! */
/*check_special_symbol.c*/
int			is_special_char(char c);
int			ft_isspace(char c);
int			ft_is_single_quote(char c, int *second_quote_found);
int			ft_is_double_quote(char c, int *second_double_found);

/*create_token_double_special_symbol.c*/
int			is_double_special(const char *input);
t_token		create_token_double_special_symbol(char **input);
void		token_heredoc_get(t_token *token, const char *delimiter, const char **environment);

/*quotes */
char	*interpret_double_quotes(const char **command_inputi, t_token_type *type);
uint32_t	determine_variables(const char *command_input);
void	extract_variable(char **variable_pointers, \
						const char *command_input, \
						const char **environement, \
						uint32_t variable_count);
void	extract_word(char **buffer, char *command_input, char **variable_pointers);
char	*interpret_single_quote(const char *command_input);

/*create_token_env_var.c*/
int			is_env_var(const char input);
t_token		create_token_env_var(char **input, const char **env);
void		extract_variable(char **variable_counter, const char *command_input, const char **environement, uint32_t variable_count);
void		variable_expand(char **input, char **buffer, const char **environement, char *temp_move);


/*create_token_quotes.c*/
int			is_quote(const char input);
t_token		create_token_single_quote(const char **input);
t_token		create_token_double_quotes(const char **input, const char **env);
t_token		create_token_quotes(const char **input, const char **env);

/*create_token_single_special_symbol.c*/
int			is_single_special(const char input);
t_token		create_token_single_special_symbol(const char **input);

/*create_token_word.c*/
t_token		create_token_word(const char **input);

/*create_token.c*/
t_token		create_token(t_token_type token_type, const char *value);
void		copy_token_info(t_token **dest, t_token *src);

/*env_utils.c*/
void		copy_input_with_new_env_var(char *new_input, const char *original_input, char *env_string, int initial_var_len);
int 		get_len_next_double_quote(const char *input, const char **env, char **new_input);

/*string_utils.c*/
int 		get_len_next_special_char(const char *input);
int 		get_len_next_space_or_quote(const char *input);
int 		get_len_next_space(const char *input);
int			get_len_next_single_quote(const char *input);

/*token_utils.c*/
void		free_token(void *addr_token);
void		free_tokens_arr(void *addr_tokens);
void		print_tokens(t_token *tokens);
void		**custom_realloc(void ***tokens, int old_capacity, int new_capacity);

/* subshell.c */
char		*execute_subshell(char *input, const char **environement);

/*tokenizer.c*/
t_token		*lexical_analysis(char *input);

/* PARSING AND AST */
/*ast_create_node.c*/
t_ast		*create_command_node(t_token_type token_type, char **args);
t_ast		*create_redireciton_node(t_token_type token_type, char *file_name);
t_ast		*create_pipe_node(t_token_type token_type);
t_ast		*create_logical_node(t_token_type token_type);

/*ats_print.c*/
void		print_ast(t_ast *head);

/*ats_utils.c*/
int			get_tokens_len(t_token *tokens);
char		**copy_args(t_ast *node, char **src);
void		append_node(t_ast **head, t_ast *new_node);
void		clear_ast(void *head);
int			is_redirection(t_token_type	token_type);
/*handle_command.c*/
void		handle_command(t_ast *current, const char **env, int *exit_status);
/*handle_fds.c*/
void		redirect_fd_into_file(t_ast *command);
void		handle_fds_child_proccess(t_ast *command);
void		handle_fds_parent_proccess(t_ast *command);

/*handle_logical_operator.c*/
void		handle_logical_operator(t_ast **logical_node, int exit_status);

/* handle_pipes.c */
void		handle_pipe(t_ast *pipe_node, int *error_catched);

/*handle_redirs.c*/
void	handle_redir_in(t_ast *branch, t_token *token, t_token *token_next);
void	handle_redir_out(t_ast *branch, t_token *token, t_token *token_next);
void	handle_redir_append(t_ast *branch, t_token *token, t_token *token_next);
void	handle_redir_heredoc(t_ast *branch, t_token *token, t_token *token_next, const char **environment);

/*parse_tokens.c*/
t_ast		*parse_tokens(t_token *tokens, const char **environment, int32_t *exit_status);

/* Syntac_check.c */
void	check_valid_logical_operator(t_token *token, int index, int *error_catched);
void	check_valid_pipe(t_token *token, int index, int *error_catched);
void	check_valid_redir(t_token *token, int index, int *error_catched);
void	print_error_redir(t_token_type token_type);
void	print_error_logical_operator(t_token_type token_type);

void	restore_fd(int original_stdin, int original_stdout);

/*parser.c*/
void		fill_args(char ***args, int count, char *token_value, int *capacity);
t_ast		parse_word(int *i, t_token *tokens);
void		parse_logical_operator(t_ast **head, int *i, t_token *tokens);
void		parse_redirection(t_ast **head, int *i, t_token *tokens);
void		parse_pipe(t_ast **head, int *i, t_token *tokens);
bool		is_delimiter_token(t_token *token);

/*syntax_check.c*/
void		check_valid_redir(t_token *token, int index,
					int *error_catched);
void		check_valid_pipe(t_token *token, int index,
					int *error_catched);
void		check_valid_logical_operator(t_token *token, int index,
							int *error_catched);

void	free_tokens(void *token_ptr);
#endif
