/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:00:46 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 11:02:54 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "minishell.h"

void			free_all(t_env *env);
void			free_for_exit(t_env *env, unsigned char exit_status);
unsigned char	free_for_exec_ast(t_env *env);

void			free_token(t_token *head, t_token *tail);
void			free_redirects(t_redirect *redirect);
void			free_list(t_list *list);
void			free_splited_data(char **data);
char			**free_allocated_data(char **datas, size_t allocated);

void			free_tree_node(t_tree_node *node);
void			free_int_matrix(int **matrix);
void			destroy_entire_tree_recursive(t_wc_tree *root);
void			**free_wc_allocated_data(t_wc_tree **datas, size_t allocated);

#endif