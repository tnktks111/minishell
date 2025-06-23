/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guruge_minishell.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-18 09:00:44 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-18 09:00:44 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GURUGE_MINISHELL_H
# define GURUGE_MINISHELL_H

# include "libft/inc/libft.h"
# include "struct.h"
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

/* lexer prototype */
// utils
bool			is_d_quote(char c);
bool			is_s_quote(char c);
bool			is_splitable(char c);
bool			is_two_word_splitable(char c1, char c2);
char			*ft_strdup_len(char *src, size_t len);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
// token utils
t_token			*new_token(char *start, size_t len);
void			append_token(t_token **head, t_token *new);
size_t			token_strlen(char *str);
size_t			in_d_quote_token_strlen(char *str);
size_t			in_s_quote_token_strlen(char *str);
// token append
size_t			append_token_and_move_index(t_token **head, char *str,
					bool d_quote, bool s_quote);
size_t			append_splitable(t_token **head, char *str);
size_t			append_two_word_splitable(t_token **head, char *str);
t_token			*tokenize_str(char *str);
// void		print_tokens(t_token *head);
t_token			*lexer(char *str, t_env *env);
bool			is_valid_input(char *input);

/* parser prototype */
// && ||
t_tree_node		*create_operator_node(t_token *op, t_tree_node *left,
					t_tree_node *right);
// | "PIPELINE"
bool			find_bang(t_token *head, t_token *tail);
t_tree_node		*create_pipeline_node(t_tree_node *pipe_root, t_token *head,
					t_token *tail);
t_tree_node		*create_pipeline_tree(t_token *head, t_token *tail);
// redirect cmd

char			*ft_strchr(const char *s, int c);
int				ft_atoi(const char *nptr);
char			**extract_args(t_token *head, t_token *tail);
t_redirect_kind	get_redirect_kind(t_token *redirect_token);
int				get_io_number(t_redirect_kind kind, t_token *redirect_token);
char			*get_output_name(t_token *redirect_token);
bool			check_if_expandable(char *filename);
void			append_redirects(t_redirect **head, t_token *redirect_token);
t_redirect		*extract_redirects(t_token *head, t_token *tail);
t_tree_node		*create_simple_cmd_node(t_token *head, t_token *tail);

t_tree_node		*create_tree(t_token *head, t_token *tail);
// utils

t_token			*skip_splitable_forward(t_token *token);
t_token			*skip_splitable_backward(t_token *token);
t_node_kind		get_node_kind(t_token *token);
t_token			*find_third_lowest_precedence_operator(t_token *head,
					t_token *tail);
t_token			*find_second_lowest_precedence_operator(t_token *head,
					t_token *tail);
t_token			*find_lowest_precedence_operator(t_token *head, t_token *tail);
t_token			*find_logical_operator(t_token *head, t_token *tail);
// paser main
t_token			*get_tail(t_token *head);
t_tree_node		*add_tree_root(t_tree_node *root);
void			free_token(t_token *head, t_token *tail);
t_tree_node		*parser(t_token *head, t_env *env);

/* expander */

size_t			_cnt_tmp_dir_file(void);
char			**gen_tmp_dir_file_array(void);

int				ft_ismatch(char *str, char *pattern, int *is_wildcard,
					size_t pat_len);
void			init_dp_table(int *dp, int *is_wildcard, size_t size);
void			ft_init_int_array(int *arr, size_t size);

void			get_cmd_line_list(t_command_line **head, char **cmd_args);
void			expand_and_append_command_line(t_command_line **head, char *str,
					char **files);
void			append_command_line(t_command_line **head, char *str);

/*errors*/
void			error_ambiguous_redirect(char *filename);
void			error_unexpected_token(char *token_str);

#endif
