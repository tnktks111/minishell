/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttanaka_minishell.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 12:13:44 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <string.h>

#define HEREDOC_SIGINT -1

/// ぬけてた
char			*join_path(char *dir, char *file);
void			find_path_failure_handler(char *cmd_name, int errnum);
/// 関数

unsigned char	builtin_cd(t_tree_node *cmd_node, t_env *env);

void			setup_interactive_signal_handlers(void);
void			setup_here_doc_signal_handlers(void);
void			setup_parent_wait_signal_handlers(void);
void			setup_child_signal_handlers(void);

void			unlink_tmpfile(t_tree_node *node_simplecmd);
void			unlink_all_tmpfiles(t_tree_node *node_pipeline);
bool			have_quotes(char *limiter);
void			remove_quotes(t_redirect *redirect);
void			here_doc_expander(char **s, t_env *env);
