/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/19 15:47:03 by ttanaka          ###   ########.fr       */
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

/*astの探査*/
unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_and_or(t_tree_node *root, t_env *env);
int				exec_pipeline(t_tree_node *root, t_env *env);
/*redirection & here_doc*/
// int						here_doc_handler(char *limiter);
void			exec_redirection(t_redirect *redirect);
void			backup_stdin_out(int *stdin_out);
void			restore_stdin_out(int *stdin_out);
/*utils*/
char			**get_path_prefix(t_env *env);
bool			is_builtin(char *s);
/*個々のコマンドの実行*/
void			find_builtin(t_tree_node *cmd_node, t_env *env);
void			find_path(t_tree_node *cmd_node, t_env *env);
unsigned char	exec_command_helper(t_tree_node *cmd_node, t_env *env);
unsigned char	exec_builtin(t_tree_node *node, t_env *env);
int				exec_solo_cmd(t_tree_node *curr, t_env *env);

unsigned char	perror_string(char *str);
void			error_cmd_not_found(char *cmd_name);
void			error_too_many_args(char *cmd_name);
void			error_numeric_arg_required(char *cmd_name, char *arg);
void			error_not_a_valid_identifier(char *cmd_name, char *arg);
void			error_illegal_arguments(char *cmd_name, char *arg);
void			error_invalid_option(char *cmd_name, char *arg);

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

#endif