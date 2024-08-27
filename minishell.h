/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:16:38 by victor            #+#    #+#             */
/*   Updated: 2024/08/27 17:28:49 by vvobis           ###   ########.fr       */
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
# include <sys/stat.h>

# define PWD 0
# define CUSTOM 1

# define PROMPT_COMMAND_STACK_SIZE 16
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

# define BUFFER_CAPACITY 64

typedef struct s_history_buffer
{
	uint	write;
	uint	read;
	uint	buffer_capacity;
	char	*buffer[BUFFER_CAPACITY];
}	t_history_buffer;

typedef struct s_prompt
{
	bool				exists;
	char				*prompt;
	t_history_buffer	history;
	uint32_t			cursor_position[2];
	void				(*prompt_display_func)(char *);
	uint32_t			prompt_length;
	char				*command;
}	t_prompt;

typedef enum e_token_type
{
	TOKEN_WORD = 1,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NEWLINE,
	TOKEN_SEMICOLON,
	TOKEN_EOL,
	TOKEN_DONE,
}	t_token_type;

typedef struct s_token
{
	t_token_type	token_type;
	char			*token_value;
}	t_token;

typedef struct s_ring_buffer
{
	uint	write;
	uint	read;
	uint	buffer_capacity;
	t_token	*buffer[BUFFER_CAPACITY];
}	t_ring_buffer;

typedef enum t_node_type
{
	NODE_NONE,
	NODE_END = TOKEN_EOL,
	NODE_PIPE = 6,
	NODE_LOGICAL_AND,
	NODE_LOGICAL_OR,
	NODE_NEWLINE,
	NODE_INVALID,
}	t_node_type;

typedef struct s_ast
{
	t_node_type				type;
	t_token_type			token_type;
	char					**args;
	bool					is_heredoc;
	char					*path_file_in;
	char					*path_file_out;
	bool					has_redir_in;
	bool					has_redir_out;
	bool					was_pipe;
	int32_t					pipefd[2];
	int						fd_in;
	int						fd_out;
	pid_t					cpid;
}	t_ast;

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

extern int32_t volatile	g_signal_flag;

/* Builtins */
int32_t		ft_echo(char **args, int32_t *exit_status);
void		ft_cd(	const char **environment, \
					const char **args, \
					int32_t *exit_status);
void		ft_pwd(const char **env, int32_t *exit_status);
void		ft_env(const char **env, int32_t *exit_status);
void		ft_unset(	char **environment, \
						const char **args, \
						int32_t *exit_status);
void		ft_export(const char **args, int32_t *exit_status);
void		ft_exit(t_ast *tree, int *exit_status);
void		*ft_realloc(void *ptr, int old_size, int new_size);

/* Commands */
bool		is_buildin(			t_ast *node);

bool		buildin_execute(	t_ast *node, \
								const char **environment, \
								int *exit_status);

void		command_execute(	char const *command_path, \
								char const *argv[], \
								char const **env);

void		m_tokenizer(		const char *input, \
								char **env, \
								int *exit_status, \
								int32_t std[2]);

/* non_interactive */
int32_t		minishell_single_command(	char *command, \
										char **environment, \
										int32_t std[2]);

void		*ft_realloc_string(		char **string, uint32_t *new_size);
char		*check_redir_input(void);

void		setup_environment(		const char **environment);

int			setup(					uint32_t argc, \
									const char **argv, \
									char **environment, \
									int32_t std[2]);

/* Handle signal */
void		handle_sigint(int sig);
void		setup_signal_handlers(void);

/* Input */
void		free_split(void *back);
uint32_t	get_split_size(const char **split);
uint32_t	get_split_length(char **split);
int			get_amount_tokens(t_token *tokens);

/* List Memory */
void		lst_memory(void *mem, void (*del)(void *c), int mode);
int			p_stderr(int stderr_fd, const char *error, const char *specifier);

/* List */
t_clean		*lst_node_new(void *content, void (*del)(void *));
void		lst_node_del(t_clean **lst);
void		lst_node_del_clean(t_clean **lst, void *mem);
void		lst_list_clean(t_clean **head);
int			lst_add_back(t_clean **node, t_clean *node_new);

/* Path Utils */
char		*find_absolute_path(	const char *path_variable, \
									char *input, \
									int *exit_status);

/* Prompt */
void		prompt_destroy(void *prompt);
t_prompt	prompt_create(uint8_t mode);
char		*prompt_get(const char *prompt);

uint32_t	prompt_display_string_set(	t_prompt *prompt, \
										const char **environment, \
										const char *prompt_string);

void		handle_arrow_key_up(	t_history_buffer *history, \
									char **input, \
									uint32_t cursor_position_current[2]);

/* Prompt_print.c */
void		prompt_print_custom_string(char *string);
void		prompt_print_pwd(char *prompt);

/* Lexer */
void		evaluate_input(	char **input[], \
							int32_t *exit_status, \
							bool *error_caught);

uint32_t	get_spaces(char *input);
uint32_t	determine_variables(char **input);
void		split_string_by_space(char *to_split, char **input_new);
void		adjust_input(char **input, char *new_string);
uint32_t	determine_full_length(char *string, char **environment);

uint32_t	evaluate_variable(	char ***input, \
								int32_t *exit_status);

/* input skip pattern */
bool		is_delimiter_variable(char c);
char		skip_to_delimiter(char *input_new);
bool		skip_single_quotes(	char *input, \
								uint32_t *i, \
								bool *in_double_quotes);
bool		evaluate_double_quotes(char *input, uint32_t *i);
bool		evaluate_single_quotes(char *input, uint32_t *i);

/* Cursor Manipulation */
void		cursor_position_get(uint32_t cursor_position[2]);
void		cursor_position_save(void);
void		cursor_position_restore(void);

bool		handle_escape_sequence(			t_prompt *prompt, \
											char buffer[], \
											char **input, \
											uint32_t cursor_position[2]);

char		*prompt_get_input(				t_prompt *prompt, \
											uint32_t prompt_initial_size, \
											const char *delimiter);

bool		handle_multiple_character(		char **input, \
											char buffer[], \
											uint32_t cursor_position[2], \
											uint32_t prompt_length);

/* Prompt Buffer Management */
void		prompt_refresh_line(			char *input, \
											uint32_t cursor_position_base, \
											uint32_t cursor_position[2]);

char		*prompt_buffer_size_manage(		char **input, \
											uint32_t old_size, \
											uint32_t size_to_add, \
											uint32_t scalar);

void		prompt_string_insert(			char *string_to_insert, \
											char **current_input, \
											char *position_to_insert, \
											uint32_t current_word_length);

bool		handle_new_character_to_input(	char **input, \
											char character, \
											uint32_t *cursor_position, \
											uint32_t prompt_length);

uint8_t		handle_single_char_input(		char **input, char buffer[], \
											uint32_t cursor_position[2], \
											bool *do_refresh);

uint8_t		handle_single_char_input(		char **input, char buffer[], \
											uint32_t cursor_position[2], \
											bool *do_refresh);

bool		handle_new_character_to_input(	char **input, \
											char character, \
											uint32_t *cursor_position, \
											uint32_t prompt_length);

/* Non Blocking mode */
void		blocking_mode_toggle(int flag);

/* Tab Completion */
void		handle_tab(				char **input, t_prompt *prompt);

void		handle_tab_no_match(	const char *input_path, \
									char **env, \
									uint32_t cursor_position_current[2], \
									t_prompt *prompt);

void		handle_tab_yes_match(	t_prompt *prompt, \
									const char *next_word_match, \
									char **input, \
									uint32_t current_word_length);

char		*determine_word(		char *input, \
									char **input_path, \
									uint32_t cursor_position_current);

void		get_next_word_match(	char **input, \
									t_prompt *prompt, \
									char *input_path, \
									bool *is_directory);

void		handle_rapid_input(		char buffer[], \
									uint32_t cursor_position[2], \
									char **input, \
									uint32_t cursor_position_base);

void		handle_backspace(		char *input, \
									uint32_t *cursor_position_current, \
									uint32_t input_length_current);

/* Termios */
void		terminal_raw_mode_enable(int flag);
void		terminal_raw_mode_disable(int flag);

/* Utils */
int			ft_close(int fd, const char *specifier);
void		ft_pipe(int pipefd[2], const char *specifier);
void		ft_dup2(int fd_old, int fd_new, const char *specifier);
void		ft_fork(pid_t *pid, const char *specifier);
void		ft_open(int *fd, const char *path, int flag, int mode);
int64_t		ft_read(int fd, char *character, uint32_t size_read);
void		ft_opendir(DIR **directory, const char *directory_path);
void		close_fds(void *std_ptr);

/* Wildcards */
void		check_and_expand_wildcards(char	***input);
int			check_wildcard(char *str);
int			get_tokens_count(char **tokens);

/* Wildcards Helper */
int			compare_suffix(	const char *current_position_in_str, \
							const char *current_position_in_pattern);

int			handle_prefix(	const char *pattern, \
							const char *str, \
							char *adr_next_wild);

int			handle_middle(	const char *pattern, const char *str);

char		**expand_wildcard(	const char *pattern);

/* Environment_variable */

void		environment_variable_print(			const char *variable, \
												const char **environment);

void		environment_print(					const char **environment);

char		**environment_create(				const char **env);

void		environment_variable_remove(		char **environment, \
												const char *variable);

void		environment_variable_add(			char ***environment, \
												const char *variable_name, \
												const char *variable_value);

char		**environment_variable_get(			const char *variable, \
												const char **environment);

char		*environment_variable_value_get(	const char *variable, \
												const char **environment);

void		environment_variable_value_change(	const char **environment, \
												const char *variable_name, \
												const char *variable_value);
char		**env_static(char **environment);

/*check_special_symbol.c*/
int			is_special_char(char c);
int			ft_isspace(char c);
bool		is_in_alphabet(char c);
bool		unrecognized_input(char c);

/*create_token_double_special_symbol.c*/
int			is_double_special(const char *input);
t_token		create_token_double_special_symbol(char **input);
bool		is_mutliple_lines(char *c);
void		remove_qoutes_delimiter(char *delimiter, uint32_t *length);
void		token_heredoc_get(	t_token *token, const char *delimiter);

/* token_heredoc.c */
char		*heredoc_while_tokenizing(char *input);
char		*handle_delimiter(char *input, char **temp_move);
char		*heredoc_loop(	char *input, \
							char *temp_move, \
							const char *delimiter, \
							uint32_t delimiter_length);

/* handle_heredoc.c */
void		handle_heredoc(t_token tokens, int32_t fd);
bool		heredoc_has_been_done(t_token *token, char *value, int fd);
void		print_value(char *value, int fd);
void		remove_quotes(char *s);

/*create_token_single_special_symbol.c*/
int			is_single_special(const char input);
t_token		create_token_single_special_symbol(const char **input);

/*create_token_word.c*/
t_token		create_token_word(const char **input);

/*create_token.c*/
t_token		create_token(t_token_type token_type, const char *value);

/*env_utils.c*/
void		print_tokens(t_token *tokens);

/* subshell.c */
char		*execute_subshell(char *input, const char **environement);

/*tokenizer.c*/
t_token		*lexical_analysis(char *input);

/* tokenizer_heredoc_helper.c */
void		ring_buffer_put(			t_ring_buffer *buffer, t_token *entry);

t_token		*ring_buffer_get(			t_ring_buffer *buffer);

t_token		mark_tokens_till_heredoc(	char *value, char **input);

void		tokenizing_loop(			char **input, \
										t_token *tokens, \
										uint32_t *i, \
										t_ring_buffer *heredoc_buffer);

/*ats_print.c*/
void		print_ast(t_ast *head);

/*handle_command.c*/

void		handle_command(		t_ast *current, \
								const char **env, \
								int *exit_status, \
								int std[2]);

void		execute_commands(	t_ast *tree, \
								const char **env, \
								int *exit_status, \
								int32_t std[2]);

void		wait_pids(			t_ast *tree, \
								uint pid_count, \
								pid_t pid_final, \
								int32_t *exit_status);

/*handle_fds.c*/
bool		handle_fds_child_proccess(t_ast *command, int32_t *exit_status);
void		handle_fds_parent_proccess(t_ast *command, int32_t *exit_status);
void		handle_pipe_in_child(t_ast *command);
void		handle_pipe_in_parent(t_ast *command);
bool		buildin_apply_pipe(t_ast *node, int32_t *exit_status);

/*handle_logical_operator.c*/
void		handle_logical_operator(t_ast **logical_node, int exit_status);

/* handle_pipes.c */
void		handle_pipe(t_ast *pipe_node, int *error_catched);

/*handle_redirs.c*/
void		handle_redir_in(		t_ast *branch, \
									t_token *token, \
									t_token *token_next);

void		handle_redir_out(		t_ast *branch, \
									t_token *token, \
									t_token *token_next);

void		handle_redir_append(	t_ast *branch, \
									t_token *token, \
									t_token *token_next);

void		handle_redir_heredoc(	t_ast *branch, \
									t_token *token, \
									uint8_t token_id);

/*parse_tokens.c*/
t_ast		*parse_tokens(	t_token *tokens, \
							int32_t *exit_status);
uint32_t	determine_trees(t_token *tokens);
void		tree_destroy(void *tree_ptr);

/* Syntac_check.c */
void		check_valid_logical_operator(	t_token *token, \
											int index, \
											int *error_catched);

void		check_valid_pipe(				t_token *token, \
											int index, \
											int *error_catched);

void		check_valid_redir(				t_token *token, \
											int index, \
											int *error_catched);

void		print_error_redir(				t_token_type token_type);

void		print_error_logical_operator(	t_token_type token_type);

void		restore_fd(int std[2]);

/*parser.c*/
void		fill_args(	char ***args, \
						int count, \
						char *token_value, \
						int *capacity);
bool		is_delimiter_token(t_token *token);

/*syntax_check.c*/
void		check_valid_redir(				t_token *token, \
											int index, \
											int *error_catched);

void		check_valid_pipe(				t_token *token, \
											int index, \
											int *error_catched);

void		check_valid_logical_operator(	t_token *token, \
											int index, \
											int *error_catched);

void		check_valid_heredoc(t_token *token, int index, int *error_catched);

/* syntax_check_helper.c */
void		print_error_logical_operator(t_token_type token_type);
void		print_error_pipe(void);
void		print_error_redir(t_token_type token_type);

void		free_tokens(void *token_ptr);
#endif
