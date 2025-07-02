/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:51:37 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 12:48:36 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/inc/libft.h"
#include "minishell.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// init.c
size_t			cnt_depth(char *pattern);
t_wildcard_type	judge_chr_type(char c, bool *in_squote, bool *in_dquote);
t_wildcard_type	*create_type_arr(char *pattern);
int				_init_pattern_and_wc_secs(t_wildcard_type *type_array,
					t_matching_info *info, char *pattern);
int				_init_matching_info(t_matching_info *info, char *pattern);

// matching.c
void			ft_int_array_swap(int *a, int *b, size_t size);
void			ft_init_int_array(int *arr, size_t size);
void			init_dp_table(int *dp, int *is_wildcard, size_t size);
int				ft_ismatch(char *str, char *pattern, int *is_wildcard,
					size_t pat_len);

// utils.c
bool			judge_contain_tail_slash(char *pattern);
char			*join_path(char *dir, char *file);
t_wc_tree		*_create_wildcard_tree_node(char *parent_path, char *filename);
size_t			_cnt_base_dir_file(char *base_dir, bool show_hidden_files);
t_wc_tree		**_gen_base_dir_file_array(char *base_dir,
					bool show_hidden_files);

// utils2.c
bool			is_hidden(char *filename);
char			*create_n_slashes(size_t n);
size_t			cnt_head_slashes(char *pattern);
bool			is_valid_pattern_chr(t_wildcard_type type);

char			*adjust_result(char *result, bool is_abs_path,
					bool contain_tail_slash);

void			destroy_entire_tree_recursive(t_wc_tree *root);

void			**free_wc_allocated_data(t_wc_tree **datas, size_t allocated);
