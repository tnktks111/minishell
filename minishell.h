/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/17 23:04:34 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "guruge_minishell.h"
# include "libft/inc/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define DEFAULT_TMPDIR "."
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif
# define RANDOM_SIZE 512
# define HASH_SIZE 1024

typedef enum e_status
{
	IN_SINGLE,
	IN_DOUBLE,
	LEFT_PAREN,
	RIGHT_PAREN,
	AND_OR,
	REDIRECT,
	SPLITABLE,
	PIPE,
	NORMAL,
	USED
}						t_status;

/*分割すべき文字...space / tab / \n / '|' / '<''>''<<''>>' / '&&' '||' / ; /*/
typedef struct s_token
{
	char				*str;
	t_status			status;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef enum e_redirect_kind
{
	REDIR_IN,     /* < */
	REDIR_OUT,    /* > */
	REDIR_APPEND, /* >> */
	REDIR_HEREDOC /* << */
}						t_redirect_kind;

typedef enum e_node_kind
{
	NODE_PIPE, /* | */
	NODE_AND,  /* '&&' */
	NODE_OR,   /* '||' */
	NODE_SIMPLE_COMMAND,
	NODE_PIPE_LINE,
	NODE_SUBSHELL,
	NODE_ROOT
}						t_node_kind;

typedef struct s_env_node
{
	bool				is_empty;
	char				*key;
	char				*val;
	struct s_env_node	*next;
}						t_env_node;

typedef struct s_env
{
	t_env_node			table[HASH_SIZE];
	char				**envp;
	unsigned char prev_exit_status; /* preserve $? */
	bool				is_malloced;
	size_t				entry_cnt;
}						t_env;

typedef struct s_pipefd
{
	int					pipefd[2];
	int					read_fd;
}						t_pipefd;

/*
> file1 2> file2 << END
のときは、
{REDIR_OUT, 1, file1, (2>file2のノード)}
{REDIR_OUT, 2, file2, (<<ENDのノード)}
{REDIR_HEREDOC, 0, END, (NULL)}
となる
*/
typedef struct s_redirect
{
	t_redirect_kind		kind;
	int					io_number;
	bool				is_expandable;
	char				*filename;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
}						t_command;

typedef struct s_pipeline
{
	bool				have_bang;
	long				exit_status;
}						t_pipeline;

typedef union u_data
{
	t_command			command;
	t_pipeline			pipeline;
}						t_data;

typedef struct s_tree_node
{
	t_node_kind			kind;
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	t_data				data;
}						t_tree_node;

int						ft_strcmp(char *s1, char *s2);

/*便利関数*/
char					*extract_key(char *s);
char					*extract_val(char *s);
char					*extract_key_when_additional(char *s);
char					*extract_val_when_additional(char *s, char *key,
							t_env *env);
char					*_concatnate_key_val(char *key, char *val,
							bool have_quote);
void					display_env(t_env *env, bool print_declare);
/*ハッシュテーブル初期化・生成・文字配列化*/
void					_init_table(t_env *env);
int						encode_envp(t_env *env, char *envp[]);
char					**decode_table(t_env *env);
/* ハッシュ値生成*/
unsigned int			_hash_fnv1a(char *str);
/* ハッシュテーブル検索*/
char					*ft_search(char *key, t_env *env);
/* チェーンノード生成・削除*/
t_env_node				*_create_env_node(char *key, char *val, t_env *env);
void					_destroy_env_node(t_env_node *node, t_env *env,
							bool is_malloced);
/*チェーンノード編集*/
int						_modify_val_of_entry(char *val, t_env_node *node);
int						_set_key_val(char *key, char *val, t_env_node *node,
							t_env *env);
/* エントリーの追加・削除*/
int						ft_add_key_val_pair(char *key, char *val, t_env *env);
void					ft_remove_key(char *key, t_env *env);
/* ハッシュテーブルのリソース解放 */
void					free_table(t_env *env);

/*astの探査*/
unsigned char			exec_ast(t_tree_node *root, t_env *env);
unsigned char			exec_and_or(t_tree_node *root, t_env *env);
unsigned char			exec_pipeline(t_tree_node *root, t_env *env);
/*redirection & here_doc*/
// int						here_doc_handler(char *limiter);
void					exec_redirection(t_redirect *redirect);
void					backup_stdin_out(int *stdin_out);
void					restore_stdin_out(int *stdin_out);
/*utils*/
char					**get_path_prefix(t_env *env);
bool					is_builtin(char *s);
/*個々のコマンドの実行*/
void					find_builtin(t_tree_node *cmd_node, t_env *env);
void					find_path(t_tree_node *cmd_node, t_env *env);
unsigned char			exec_command_helper(t_tree_node *cmd_node, t_env *env);
unsigned char			exec_builtin(t_tree_node *node, t_env *env);
unsigned char			exec_solo_cmd(t_tree_node *curr, t_env *env);

unsigned char			perror_string(char *str);
void					error_cmd_not_found(char *cmd_name);
void					error_too_many_args(char *cmd_name);
void					error_numeric_arg_required(char *cmd_name, char *arg);
void					error_not_a_valid_identifier(char *cmd_name, char *arg);
void					error_illegal_arguments(char *cmd_name, char *arg);
void					error_invalid_option(char *cmd_name, char *arg);

unsigned char			builtin_echo(t_tree_node *node, t_env *env);
unsigned char			builtin_pwd(t_tree_node *node, t_env *env);
unsigned char			builtin_export(t_tree_node *node, t_env *env);
unsigned char			builtin_unset(t_tree_node *node, t_env *env);
unsigned char			builtin_env(t_tree_node *node, t_env *env);
unsigned char			builtin_exit(t_tree_node *node, t_env *env);

int						sh_mktmpfd(char **file_path_ptr);

void					free_splited_data(char **data);
char					**free_allocated_data(char **datas, size_t allocated);

void					print_tree(t_tree_node *node);

#endif