/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/10 20:44:11 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>

typedef enum e_status
{
	IN_SINGLE,
	IN_DOUBLE,
	NORMAL
}					t_status;

/*分割すべき文字...space / tab / \n / '|' / '<''>''<<''>>' / '&&' '||' / ; /*/
typedef struct s_token
{
	char			*str;
	bool			is_meta;
	t_status		status;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef enum e_redirect_kind{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
} t_redirect_kind;

typedef enum e_node_kind{
	NODE_PIPE, /* | */
	NODE_AND, /* '&&' */
	NODE_OR,  /* '||' */
	NODE_SIMPLE_COMMAND,
	NODE_PIPE_LINE
} t_node_kind;

typedef struct s_redirect{
	t_redirect_kind kind;
	int io_number;
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

union u_data{
	t_command command;
	t_pipeline pipeline;
};

typedef struct s_tree_node{
	t_node_kind kind;
	struct s_tree_node *left;
	struct s_tree_node *right;
	u_data data;
} t_tree_node;

#endif

