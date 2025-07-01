/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttanaka_minishell.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 22:14:10 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <string.h>

#define HEREDOC_SIGINT -1
#define TMPFILE_CHR 	"abcdefghijklmnopqrstuvwxyz \
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

/// ぬけてた
char			*join_path(char *dir, char *file);
void			find_path_failure_handler(char *cmd_name, int errnum,
					t_env *env);
/// 関数

unsigned char	builtin_cd(t_tree_node *cmd_node, t_env *env);

void			setup_interactive_signal_handlers(void);
void			setup_here_doc_signal_handlers(void);
void			setup_parent_wait_signal_handlers(void);
void			setup_child_signal_handlers(void);

void			unlink_tmpfile(t_tree_node *node_simplecmd);
void			unlink_all_tmpfiles(t_tree_node *node_pipeline);
bool			have_quotes(char *limiter);
int				remove_quotes(t_redirect *redirect);
char			*here_doc_expander(char *s, t_env *env);

void			free_int_matrix(int **matrix);
void			free_for_exit(t_env *env, unsigned char exit_status);
unsigned char	free_for_exec_ast(t_env *env);

void			close_pipefd(int *fd);
