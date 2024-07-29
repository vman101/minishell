/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:16:38 by victor            #+#    #+#             */
/*   Updated: 2024/07/29 12:03:34 by anarama          ###   ########.fr       */
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
# include <signal.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>

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

# define INITIAL_TOKEN_CAPACITY 16

typedef struct s_prompt
{
	char		*command;
	uint32_t	prompt_length;
	uint32_t	history_position_current;
	uint32_t	history_count;
	char		**history_entries;
	char		**env_ptr;
}				t_prompt;

typedef enum
{
	TOKEN_WORD,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
	TOKEN_EOL,
    TOKEN_ENV,
    TOKEN_EXIT_STATUS
} t_token_type;

typedef struct s_token
{
	t_token_type	token_type;
	char			*token_value;
}	t_token;

typedef enum 
{
    NODE_COMMAND,
    NODE_REDIRECTION,
    NODE_PIPE,
    NODE_LOGICAL_OPERATOR,
} t_node_type;

typedef struct s_ast 
{
    t_node_type		type;
	t_token_type	token_type;
	struct s_ast	*left;
    struct s_ast	*right;
    char 			**args;
    char			*file;
	char			*error_message;
	char			*path;
	int				fd_file_in;
	int				fd_file_out;
    int				fd_in;
	int				fd_out;
	int				std_fd;
	int				flags;
	int				is_done;
	int				error_found;
	int				exit_status;
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
uint32_t	prompt_display();
t_prompt	*prompt_create(const char **env);
char		*prompt_get(t_prompt *prompt);

/* Cursor Manipulation */
void		cursor_position_get(uint32_t cursor_position[2]);
void		cursor_position_save(void);
void		cursor_position_restore(void);

uint8_t		handle_escape_sequence(t_prompt *prompt, char **input, uint32_t *cursor_position_current);
char		*prompt_get_input(t_prompt *prompt);

/* Prompt Buffer Management */
void		prompt_refresh_line(char *input, uint32_t cursor_position_current[2]);
void		prompt_buffer_size_manage(char **input, uint32_t input_new_size);
void		prompt_string_insert(char *string_to_insert, char **current_input, char *position_to_insert, uint32_t current_word_length);

/* Redirections */
void		execute(char **tokens, char **env);
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
int			get_tokens_count(char **tokens);
void		custom_free_split(char **arr);
void		handle_dollar_sign(char **single_token, char **env);
void		ft_echo(char **tokens, int fd);
void		ft_pwd(int fd, const char **env);
void		ft_env(char **env, int fd);

/* Environment_variable */

void		environment_variable_print(const char *variable, const char **environment);
void		environment_print(char **environment);
char		**environment_create(const char **env);
char		**environment_variable_add(char **environment, const char *variable_new_name, const char *variable_new_value);
char		*environment_variable_get(const char *variable, const char **environment);

/* TOKENIZER MOTHERFUCKER!!! */
/*check_special_symbol.c*/
int			is_special_char(char c);
int			ft_isspace(char c);
int			ft_is_single_quote(char c, int *second_quote_found);
int			ft_is_double_quote(char c, int *second_double_found);

/*create_token_double_special_symbol.c*/
int			is_double_special(const char *input);
t_token		create_token_double_special_symbol(char **input);

/*quotes */
char	*interpret_double_quotes(const char **command_input, const char **environement, t_token_type *type);
uint32_t	determine_variables(const char *command_input);
void	extract_variable(char **variable_pointers, \
						const char *command_input, \
						const char **environement, \
						uint32_t variable_count);
char	*extract_word(char *command_input, char **variable_pointers);
char	*interpret_single_quote(const char *command_input);

/*create_token_env_var.c*/
int			is_env_var(const char *input);
t_token		create_token_env_var(char **input, const char **env);
void		extract_variable(char **variable_counter, const char *command_input, const char **environement, uint32_t variable_count);

/*create_token_quotes.c*/
int			is_quote(const char *input);
t_token		create_token_single_quote(const char **input);
t_token		create_token_double_quotes(const char **input, const char **env);
t_token		create_token_quotes(const char **input, const char **env);

/*create_token_single_special_symbol.c*/
int			is_single_special(const char *input);
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
/*tokenizer.c*/
t_token		*lexical_analysis(const char *input, const char **env);

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
void	handle_command(t_ast *current, const char *path_variable,
					const char **env, int *exit_status);
/*handle_fds.c*/
void	redirect_fd_into_file(t_ast *command);
void	handle_fds_child_proccess(t_ast *command);
void	handle_fds_parent_proccess(t_ast *command);
/*handle_logical_operator.c*/
t_ast *skip_up_to_next_logical_operator(t_ast *ast);
void		handle_logical_operator(t_ast *logical_node, int exit_status);
/* handle_pipes.c */
void		handle_pipe(t_ast *pipe_node);
/*handle_redirs.c*/
void		handle_redir(t_ast *redir_node, t_ast **head);
/*syntax_checks.c*/
void	check_valid_redir(t_ast *redir_node, int *error_catched);
void	check_valid_pipe(t_ast *pipe_node, int *error_catched);
void	check_valid_logical_operator(t_ast *logical_node, int *error_catched);
/*parse_tokens.c*/
t_ast		*parse_tokens(t_token *tokens);

void	restore_fd(int original_stdin, int original_stdout);
/*parser.c*/
void		fill_args(char **args, int count, char *token_value, int *capacity);
void		parse_word(t_ast **head, int *i, t_token *tokens);
void		parse_logical_operator(t_ast **head, int *i, t_token *tokens);
void		parse_redirection(t_ast **head, int *i, t_token *tokens);
void		parse_pipe(t_ast **head, int *i, t_token *tokens);
#endif
