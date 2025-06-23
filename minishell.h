/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/23 10:52:39 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# include "guruge_minishell.h"
# include "libft/inc/libft.h"
# include "struct.h"
# include "ttanaka_minishell.h"
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

int				ft_strcmp(char *s1, char *s2);

/*便利関数*/
char			*extract_key(char *s);
char			*extract_val(char *s);
char			*extract_key_when_additional(char *s);
char			*extract_val_when_additional(char *s, char *key, t_env *env);
char			*_concatnate_key_val(char *key, char *val, bool have_quote);
void			display_env(t_env *env, bool print_declare);
/*ハッシュテーブル初期化・生成・文字配列化*/
void			_init_table(t_env *env);
int				encode_envp(t_env *env, char *envp[]);
char			**decode_table(t_env *env);
/* ハッシュ値生成*/
unsigned int	_hash_fnv1a(char *str);
/* ハッシュテーブル検索*/
char			*ft_search(char *key, t_env *env);
/* チェーンノード生成・削除*/
t_env_node		*_create_env_node(char *key, char *val, t_env *env);
void			_destroy_env_node(t_env_node *node, t_env *env,
					bool is_malloced);
/*チェーンノード編集*/
int				_modify_val_of_entry(char *val, t_env_node *node);
int				_set_key_val(char *key, char *val, t_env_node *node,
					t_env *env);
/* エントリーの追加・削除*/
int				ft_add_key_val_pair(char *key, char *val, t_env *env);
void			ft_remove_key(char *key, t_env *env);
/* ハッシュテーブルのリソース解放 */
void			free_table(t_env *env);

/*execution*/
unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_and_or(t_tree_node *root, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid);
int				exec_pipeline(t_tree_node *root, t_env *env);
unsigned char	exec_command_helper(t_tree_node *cmd_node, t_env *env);

int				exec_solo_cmd(t_tree_node *curr, t_env *env);

unsigned char	exec_builtin(t_tree_node *node, t_env *env);
void			exec_redirection(t_redirect *redirect);

void			setup_pipefd(t_pipefd *fd, t_tree_node *node, bool is_start);
int				status_handler(int status);
char			**get_path_prefix(t_env *env);
void			find_builtin(t_tree_node *cmd_node, t_env *env);
void			find_path(t_tree_node *cmd_node, t_env *env);
bool			is_builtin(char *s);

/*here_doc*/
int		prepare_here_doc(t_tree_node *node, t_env *env);
char	*here_doc_handler(char *limiter, t_env *env);
void	here_doc_expander(char **s, t_env *env);
bool	have_quotes(char *limiter);
void	remove_quotes(char **limiter);
void	unlink_tmpfile(t_tree_node *node_simplecmd);
void	unlink_all_tmpfiles(t_tree_node *node_pipeline);

unsigned char	perror_string(char *str);
void			error_cmd_not_found(char *cmd_name);
void			error_too_many_args(char *cmd_name);
void			error_numeric_arg_required(char *cmd_name, char *arg);
void			error_not_a_valid_identifier(char *cmd_name, char *arg);
void			error_illegal_arguments(char *cmd_name, char *arg);
void			error_invalid_option(char *cmd_name, char *arg);
void 			builtin_error(char *cmd_name, char *arg, char *errmsg);
void 			warning_shlvl_too_high(int shlvl);

unsigned char	builtin_echo(t_tree_node *node, t_env *env);
unsigned char	builtin_pwd(t_tree_node *node, t_env *env);
unsigned char	builtin_export(t_tree_node *node, t_env *env);
unsigned char	builtin_unset(t_tree_node *node, t_env *env);
unsigned char	builtin_env(t_tree_node *node, t_env *env);
unsigned char	builtin_exit(t_tree_node *node, t_env *env);
int				sh_mktmpfd(char **file_path_ptr);

void			free_splited_data(char **data);
char			**free_allocated_data(char **datas, size_t allocated);

void			print_tree(t_tree_node *node);
void			expand_ast(t_tree_node *node, t_env *env);

#endif