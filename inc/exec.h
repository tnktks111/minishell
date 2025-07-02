/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:36:59 by sguruge           #+#    #+#             */
/*   Updated: 2025/07/02 17:18:56 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

# define HEREDOC_SIGINT -1
# define TMPFILE_CHR \
	"abcdefghijklmnopqrstuvwxyz \
	ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

/*envp*/
char			*extract_key(char *s);
char			*extract_val(char *s);
char			*extract_key_when_additional(char *s);
char			*extract_val_when_additional(char *s, char *key, t_env *env);
char			*_concatenate_key_val(char *key, char *val, bool have_quote);
void			display_env(t_env *env, bool print_declare);
void			_init_table(t_env *env);
int				encode_envp(t_env *env, char *envp[]);
char			**decode_table(t_env *env, bool include_quote);
unsigned int	_hash_fnv1a(char *str);
char			*ft_search(char *key, t_env *env);
t_env_node		*_create_env_node(char *key, char *val, t_env *env);
void			_destroy_env_node(t_env_node *node, t_env *env,
					bool envp_is_malloced);
int				_modify_val_of_entry(char *val, t_env_node *node);
int				_set_key_val(char *key, char *val, t_env_node *node,
					t_env *env);
int				ft_add_key_val_pair(char *key, char *val, t_env *env);
int				ft_set_underscore(t_tree_node *cmd_node, t_env *env);
void			ft_remove_key(char *key, t_env *env);
void			free_table(t_env *env);

/*execution*/
unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_pipeline(t_tree_node *node_pipeline, t_env *env);
int				exec_pl_cmds(t_tree_node *node_pipeline, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env,
					pid_t *lastpid);
void			exec_command_helper(t_tree_node *cmd_node, t_env *env);
int				exec_solo_cmd(t_tree_node *curr, t_env *env);
int				wcoredump(int status);
void			setup_pipefd(t_pipefd *fd, t_tree_node *node, bool is_start);
void			close_pipefd(int *fd);
int				status_handler(int status);
bool			is_directory(char *path);
char			**get_path_prefix(t_env *env);
void			find_builtin(t_tree_node *cmd_node, t_env *env);
void			find_path(char **args, t_env *env);
bool			is_builtin(char *s);
int				*save_std_fds(void);
void			restore_std_fds(int *saved_std_fds);
void			find_path_failure_handler(char *cmd_name, int errnum,
					t_env *env);
void			execve_failure_handler(char *cmd_name, int errnum, t_env *env);
void			n_wait(size_t n);
int				exec_redirection(t_redirect *redirect);
unsigned char	exec_builtin(t_tree_node *node, t_env *env, int *saved_std_fds);
void			setup_interactive_signal_handlers(void);
void			setup_here_doc_signal_handlers(void);
void			setup_parent_wait_signal_handlers(void);
void			setup_child_signal_handlers(void);

/*here_doc*/
int				prepare_here_doc(t_tree_node *node, t_env *env);
char			*here_doc_handler(t_redirect *redirect, t_env *env);
int				prepare_entire_here_docs(t_tree_node *root, t_env *env);
char			*here_doc_expander(char *s, t_env *env);
bool			have_quotes(char *limiter);
int				remove_quotes(t_redirect *redirect);
int				sh_mktmpfd(char **file_path_ptr);
int				file_iswdir(char *filepath);
void			unlink_tmpfile(t_tree_node *node_simplecmd);
void			unlink_pl_tmpfiles(t_tree_node *node_pipeline);
void			unlink_all_tmpfiles(t_tree_node *node);

/*builtin*/
/*cd*/
unsigned char	builtin_cd(t_tree_node *cmd_node, t_env *env);
char			*make_absolute(char *path);
int				bindpwd(t_env *env);
int				absolute_pathname(const char *string);
int				change_to_directory(char *path);
void			free_for_find_cdpath(char *s1, char *s2, char **splitted_data);
/*others*/
unsigned char	builtin_echo(t_tree_node *node, t_env *env);
unsigned char	builtin_env(t_tree_node *node, t_env *env);
unsigned char	builtin_exit(t_tree_node *node, t_env *env, int *saved_std_fds);
unsigned char	builtin_export(t_tree_node *node, t_env *env);
unsigned char	builtin_pwd(t_tree_node *node, t_env *env);
unsigned char	builtin_unset(t_tree_node *node, t_env *env);

#endif
