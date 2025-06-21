#ifndef STRUCT_H
# define STRUCT_H

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
# define RANDOM_SIZE 10
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
}							t_status;

/*分割すべき文字...space / tab / \n / '|' / '<''>''<<''>>' / '&&' '||' / ; /*/
typedef struct s_token
{
	char					*str;
	t_status				status;
	struct s_token			*next;
	struct s_token			*prev;
}							t_token;

typedef enum e_redirect_kind
{
	REDIR_IN,     /* < */
	REDIR_OUT,    /* > */
	REDIR_APPEND, /* >> */
	REDIR_HEREDOC /* << */
}							t_redirect_kind;

typedef enum e_node_kind
{
	NODE_PIPE, /* | */
	NODE_AND,  /* '&&' */
	NODE_OR,   /* '||' */
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

typedef struct s_env
{
	t_env_node				table[HASH_SIZE];
	char					**envp;
	int prev_exit_status; /* preserve $? */
	bool					is_malloced;
	size_t					entry_cnt;
}							t_env;

typedef struct s_pipefd
{
	int						pipefd[2];
	int						read_fd;
}							t_pipefd;

typedef struct s_command_line
{
	char					*arg;
	struct s_command_line	*next;
}							t_command_line;

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

#endif
