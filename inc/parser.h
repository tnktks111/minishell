/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-02 00:19:08 by sguruge           #+#    #+#             */
/*   Updated: 2025-07-02 00:19:08 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

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

# define ERROR_REDIR 1

/* lexer prototype */

// token_fix
bool			check_for_unclosed_quotes(char *str, t_env *env);
bool			is_mergeable(t_status status);
t_status		get_status(t_status status1, t_status status2);
void			fix_non_splitted_tokens(t_token **head);
// utils
bool			is_d_quote(char c);
bool			is_s_quote(char c);
bool			is_splitable(char c);
bool			is_space(char c);
bool			is_param(char c);
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
size_t			append_param_token_and_move_index(t_token **head, char *str);
size_t			append_splitable(t_token **head, char *str);
size_t			append_two_word_splitable(t_token **head, char *str);
t_token			*tokenize_str(char *str);
t_token			*lexer(char *str);
bool			is_valid_input(char *input, t_env *env);

/* parser prototype */
// && ||
t_tree_node		*create_operator_node(t_token *op, t_tree_node *left,
					t_tree_node *right);
// | "PIPELINE"
bool			find_bang(t_token *head, t_token *tail);
t_tree_node		*create_pipeline_node(t_tree_node *left, t_token *head,
					t_token *tail);
t_tree_node		*create_pipeline_tree(t_token *head, t_token *tail,
					t_create_tree *tree);
// redirect cmd
char			**extract_args(t_token *head, t_token *tail);
t_redirect_kind	get_redirect_kind(t_token *redirect_token);
int				get_io_number(t_redirect_kind kind, t_token *redirect_token);
char			*get_output_name(t_token *redirect_token);
bool			check_if_expandable(char *filename);
void			append_redirects(t_redirect **head, t_token *redirect_token);
t_redirect		*extract_redirects(t_token *head, t_token *tail);
t_tree_node		*create_simple_cmd_node(t_token *head, t_token *tail);
//()()
t_tree_node		*parse_paren(t_create_tree *tree, t_token *head, t_token *tail);
t_token			*get_redir_tail(t_token *head, t_token *tail);
t_tree_node		*create_subshell_node(t_tree_node *cur_root, t_token *head,
					t_token *tail);
//
t_tree_node		*create_tree(t_token *head, t_token *tail);
// utils
t_token			*find_matching_paren(t_token *head);
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

int				ft_ismatch(char *str, char *pattern, int *is_wildcard,
					size_t pat_len);
void			init_dp_table(int *dp, int *is_wildcard, size_t size);
void			ft_init_int_array(int *arr, size_t size);

int				get_cmd_line_list(t_list **head, char **cmd_args);
int				expand_and_append_command_line(char *str, t_list **head);
int				append_command_line(t_list **head, char *str);
char			**list_to_args(t_list *head);

size_t			count_args(char **args);
bool			check_variable_expand(char *str);
char			*get_variable_str(char *src);
char			*expand_individual_variable(char *str, t_env *env);
char			*expand_every_variable(char *str, t_env *env);
char			**expand_cmd_variable(char **cmd_args, t_env *env);

int				handle_file_wildcard(t_redirect *cur, char *temp,
					char *redir_file);
bool			check_wildcard_expand(char *str);
char			**expand_cmd_wildcard(char **cmd_args);
void			init_wildcard_array(int *src, char *str, size_t str_len);
char			*expand_filename_wildcard(char *filename, bool *error);
char			**expand_cmd_line(t_list *cmdline);
char			**expand_file_line(t_list *fileline);

void			takeoff_quotes(char *str);
int				expand_filename(t_redirect *simple_cmd_node, t_env *env);
void			expand_cmd_args(t_tree_node *simple_cmd_node, t_env *env);
int				expander(t_tree_node *pipeline_node, t_env *env);
int				expand_ast(t_tree_node *node, t_env *env);

int				ft_glob(char *pattern, t_list **res_head);
void			free_list(t_list *list);
void			free_redirects(t_redirect *redirect);
void			free_tree_node(t_tree_node *node);
void			free_all(t_env *env);

/*errors*/
bool			check_syntax_error(t_token *head);
bool			check_paren_syntax_error(t_token *head);
bool			check_closed_paren_syntax_error(t_token *head);
bool			check_cosecutive_paren_syntax_error(t_token *head);
void			handle_syntax_error(t_env *env);
void			error_redir(char *filename);
void			error_unexpected_token(char *token_str);
bool			is_status_meta(t_status status);
bool			is_status_splitable(t_status status);
bool			is_status_paren(t_status status);
bool			check_op_placement(t_token *head);
bool			check_redirect_placement(t_token *head);

#endif
