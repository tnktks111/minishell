/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/07/01 17:18:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include "guruge_minishell.h"
# include "libft/inc/libft.h"
# include "struct.h"
# include "ttanaka_minishell.h"
# include "wildcard.h"
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

/*便利関数*/
char			*extract_key(char *s);
char			*extract_val(char *s);
char			*extract_key_when_additional(char *s);
char			*extract_val_when_additional(char *s, char *key, t_env *env);
char			*_concatenate_key_val(char *key, char *val, bool have_quote);
void			display_env(t_env *env, bool print_declare);
/*ハッシュテーブル初期化・生成・文字配列化*/
void			_init_table(t_env *env);
int				encode_envp(t_env *env, char *envp[]);
char			**decode_table(t_env *env, bool include_quote);
/* ハッシュ値生成*/
unsigned int	_hash_fnv1a(char *str);
/* ハッシュテーブル検索*/
char			*ft_search(char *key, t_env *env);
/* チェーンノード生成・削除*/
t_env_node		*_create_env_node(char *key, char *val, t_env *env);
void			_destroy_env_node(t_env_node *node, t_env *env,
					bool envp_is_malloced);
/*チェーンノード編集*/
int				_modify_val_of_entry(char *val, t_env_node *node);
int				_set_key_val(char *key, char *val, t_env_node *node,
					t_env *env);
/* エントリーの追加・削除*/
int				ft_add_key_val_pair(char *key, char *val, t_env *env);
int				ft_set_underscore(t_tree_node *cmd_node, t_env *env);
void			ft_remove_key(char *key, t_env *env);
/* ハッシュテーブルのリソース解放 */
void			free_table(t_env *env);

/*execution*/
unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env,
					pid_t *lastpid);
int				exec_pipeline(t_tree_node *root, t_env *env);
int				exec_pl_cmds(t_tree_node *root, t_env *env);
void			exec_command_helper(t_tree_node *cmd_node, t_env *env);

int				exec_solo_cmd(t_tree_node *curr, t_env *env);

unsigned char	exec_builtin(t_tree_node *node, t_env *env, int *saved_std_fds);
int				exec_redirection(t_redirect *redirect);

int				wcoredump(int status);
void			setup_pipefd(t_pipefd *fd, t_tree_node *node, bool is_start);
int				status_handler(int status);
char			**get_path_prefix(t_env *env);
void			find_builtin(t_tree_node *cmd_node, t_env *env);
void			find_path(char **args, t_env *env);
bool			is_builtin(char *s);
bool			is_directory(char *path);
int				*save_std_fds(void);
void			restore_std_fds(int *saved_fds);
void			find_path_failure_handler(char *cmd_name, int errnum,
					t_env *env);
void			execve_failure_handler(char *cmd_name, int errnum, t_env *env);

/*here_doc*/
int				prepare_here_doc(t_tree_node *node, t_env *env);
char			*here_doc_handler(t_redirect *redirect, t_env *env);
char			*here_doc_expander(char *s, t_env *env);
bool			have_quotes(char *limiter);
int				remove_quotes(t_redirect *redirect);
void			unlink_tmpfile(t_tree_node *node_simplecmd);
void			unlink_all_tmpfiles(t_tree_node *node_pipeline);
int				file_iswdir(char *filepath);

void			perror_string(char *str);
void			ft_puterr_general(char *cmd_name, char *errmsg);
void			ft_puterr_nbr(int nbr, char *errmsg);
void			error_too_many_args(char *cmd_name);
void			error_numeric_arg_required(char *cmd_name, char *arg);
void			error_not_a_valid_identifier(char *cmd_name, char *arg);
void			error_illegal_arguments(char *cmd_name, char *arg);
void			error_invalid_option(char *cmd_name, char *arg);
void			builtin_error(char *cmd_name, char *arg, char *errmsg);
void			warning_shlvl_too_high(int shlvl);
void			error_heredoc_delimited_by_eof(char *delimiter);
void			*ft_puterr_malloc(void);

unsigned char	builtin_echo(t_tree_node *node, t_env *env);
unsigned char	builtin_pwd(t_tree_node *node, t_env *env);
unsigned char	builtin_export(t_tree_node *node, t_env *env);
unsigned char	builtin_unset(t_tree_node *node, t_env *env);
unsigned char	builtin_env(t_tree_node *node, t_env *env);
unsigned char	builtin_exit(t_tree_node *node, t_env *env, int *saved_std_fds);
int				sh_mktmpfd(char **file_path_ptr);

int				bindpwd(t_env *env);
int				absolute_pathname(const char *string);
char			*make_absolute(char *path);
int				change_to_directory(char *path);
void			free_for_find_cdpath(char *s1, char *s2, char **splitted_data);

void			free_splited_data(char **data);
char			**free_allocated_data(char **datas, size_t allocated);

void			expand_ast(t_tree_node *node, t_env *env);

void			print_tree(t_tree_node *node);
void			print_tokens(t_token *head);

#endif