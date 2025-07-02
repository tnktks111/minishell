/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:46:19 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 10:56:13 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "minishell.h"

void	perror_string(char *str);
void	ft_puterr_general(char *cmd_name, char *errmsg);
void	ft_puterr_nbr(int nbr, char *errmsg);
void	*ft_puterr_malloc(void);

void	error_invalid_option(char *cmd_name, char *arg);
void	builtin_error(char *cmd_name, char *arg, char *errmsg);
void	warning_shlvl_too_high(int shlvl);
void	error_heredoc_delimited_by_eof(char *delimiter);

void	error_too_many_args(char *cmd_name);
void	error_numeric_arg_required(char *cmd_name, char *arg);
void	error_not_a_valid_identifier(char *cmd_name, char *arg);
void	error_illegal_arguments(char *cmd_name, char *arg);

bool	check_syntax_error(t_token *head);
bool	check_paren_syntax_error(t_token *head);
bool	check_closed_paren_syntax_error(t_token *head);
bool	check_cosecutive_paren_syntax_error(t_token *head);
void	handle_syntax_error(t_env *env);
bool	is_status_meta(t_status status);
bool	is_status_splitable(t_status status);
bool	is_status_paren(t_status status);
bool	check_op_placement(t_token *head);
bool	check_redirect_placement(t_token *head);

void	error_redir(char *filename);
void	error_unexpected_token(char *token_str);

#endif