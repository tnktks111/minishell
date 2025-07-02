/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:05:14 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 13:17:38 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void			free_all(t_env *env);
void			free_for_exit(t_env *env, unsigned char exit_status);
unsigned char	free_for_exec_ast(t_env *env);

void	free_all(t_env *env)
{
	free_table(env);
	if (env->root)
		free_tree_node(env->root);
}

void	free_for_exit(t_env *env, unsigned char exit_status)
{
	if (env->is_child == false)
		unlink_all_tmpfiles(env->root);
	free_all(env);
	exit(exit_status);
}

unsigned char	free_for_exec_ast(t_env *env)
{
	if (env->is_child == false)
		unlink_all_tmpfiles(env->root);
	if (env->root)
		free_tree_node(env->root);
	return (env->prev_exit_status);
}
