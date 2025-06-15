/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/15 22:05:26 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/errno.h>
#include <limits.h>

#define DEFAULT_TMPDIR "."
#define PATH_MAX 1024
#define RANDOM_SIZE 512
#define HASH_SIZE 1024

typedef enum e_status
{
	IN_SINGLE,
	IN_DOUBLE,
	NORMAL,
	SPLITABLE
}					t_status;

/*分割すべき文字...space / tab / \n / '|' / '<''>''<<''>>' / '&&' '||' / ; /*/
typedef struct s_token
{
	char			*str;
	t_status		status;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef enum e_redirect_kind{
	REDIR_IN, /* < */
	REDIR_OUT, /* > */
	REDIR_APPEND, /* >> */
	REDIR_HEREDOC /* << */
} t_redirect_kind;

typedef enum e_node_kind{
	NODE_PIPE, /* | */
	NODE_AND, /* '&&' */
	NODE_OR,  /* '||' */
	NODE_SIMPLE_COMMAND,
	NODE_PIPE_LINE,
	NODE_ROOT
} t_node_kind;

typedef struct s_env_node
{
    bool is_empty;
    char *key;
    char *val;
    struct s_env_node *next;
} t_env_node;

typedef struct s_env
{
	t_env_node table[HASH_SIZE];
	char **envp;
	unsigned char prev_exit_status; /* preserve $? */
	bool is_malloced;
	size_t entry_cnt;
} t_env;

typedef struct s_pipefd
{
	int pipefd[2];
	int read_fd;
} t_pipefd;

/*
> file1 2> file2 << END
のときは、
{REDIR_OUT, 1, file1, (2>file2のノード)}
{REDIR_OUT, 2, file2, (<<ENDのノード)}
{REDIR_HEREDOC, 0, END, (NULL)}
となる
*/
typedef struct s_redirect{
	t_redirect_kind kind;
	int io_number;
	bool is_expandable;
	char *filename;
	struct s_redirect *next;
} t_redirect;

typedef struct s_command{
	char **args;
	t_redirect *redirects;
} t_command;

typedef struct s_pipeline{
	bool have_bang;
	long exit_status;
} t_pipeline;

typedef union u_data{
	t_command command;
	t_pipeline pipeline;
} t_data;

typedef struct s_tree_node{
	t_node_kind kind;
	struct s_tree_node *parent;
	struct s_tree_node *left;
	struct s_tree_node *right;
	t_data data;
} t_tree_node;

#endif