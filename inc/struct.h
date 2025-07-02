/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 04:53:42 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/29 15:55:04 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

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
# define RANDOM_SIZE 10
# define HASH_SIZE 1024

typedef enum e_wildcard_type
{
	PLAIN_CHR,
	EXPANDABLE_ASTERISK,
	QUOTE,
	SLASH
}							t_wildcard_type;

typedef struct s_wildcard_tree
{
	char					*path;
	char					*filename;
	struct s_wildcard_tree	*parent;
	struct s_wildcard_tree	**children;
}							t_wc_tree;

typedef struct s_split_vars
{
	size_t					l;
	size_t					r;
	size_t					idx;
	size_t					cnt;
}							t_split_vars;

typedef struct s_matching_info
{
	t_list					*res;
	int						**is_wildcard;
	char					**ptn_secs;
	size_t					depth;
	int						total_cnt;
	size_t					head_slash_cnt;
	bool					contain_tail_slash;
	bool					is_abs_path;
	bool					error_happened;
}							t_matching_info;

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
}							t_status;

typedef struct s_token
{
	char					*str;
	t_status				status;
	struct s_token			*next;
	struct s_token			*prev;
}							t_token;

typedef enum e_redirect_kind
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}							t_redirect_kind;

typedef enum e_node_kind
{
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SIMPLE_COMMAND,
	NODE_PIPE_LINE,
	NODE_SUBSHELL,
	NODE_ROOT
}							t_node_kind;

typedef struct s_env_node
{
	bool					is_empty;
	char					*key;
	char					*val;
	struct s_env_node		*next;
}							t_env_node;

typedef struct s_pipefd
{
	int						pipefd[2];
	int						read_fd;
}							t_pipefd;

typedef struct s_redirect
{
	t_redirect_kind			kind;
	int						io_number;
	bool					is_expandable;
	char					*filename;
	struct s_redirect		*next;
}							t_redirect;

typedef struct s_command
{
	char					**args;
	t_redirect				*redirects;
}							t_command;

typedef struct s_pipeline
{
	bool					have_bang;
	long					exit_status;
}							t_pipeline;

typedef union u_data
{
	t_command				command;
	t_pipeline				pipeline;
}							t_data;

typedef struct s_tree_node
{
	t_node_kind				kind;
	struct s_tree_node		*parent;
	struct s_tree_node		*left;
	struct s_tree_node		*right;
	t_data					data;
}							t_tree_node;

typedef struct s_create_tree
{
	t_tree_node				*left;
	t_tree_node				*right;
	t_tree_node				*pipeline_root;
	t_tree_node				*paratheneses_root;
}							t_create_tree;

typedef struct s_env
{
	t_env_node				table[HASH_SIZE];
	char					**envp;
	int						prev_exit_status;
	bool					is_child;
	bool					envp_is_malloced;
	size_t					entry_cnt;
	t_tree_node				*root;
}							t_env;

#endif
