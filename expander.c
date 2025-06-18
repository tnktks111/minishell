/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/17 22:18:40 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// unsigned char	expander(t_tree_node *root, t_env *env)
// {
// 	t_tree_node	*curr;
// 	pid_t		pid;
// 	size_t		cnt;
// 	t_pipefd	fd;
// 	int			status;

// 	curr = root;
// 	cnt = 0;
// 	fd.read_fd = STDIN_FILENO;
// 	if (curr->kind == NODE_SIMPLE_COMMAND)
// 		return (exec_solo_cmd(curr, env));
// 	while (curr->kind == NODE_PIPE)
// 		curr = curr->left;
// 	while (curr->kind != NODE_PIPE_LINE)
// 	{
// 		if (curr->parent->kind == NODE_PIPE && pipe(fd.pipefd) == -1)
// 			return (perror_string("pipe: "));
// 		cnt++;
// 		pid = fork();
// 		if (pid == -1)
// 			return (perror_string("fork: "));
// 		if (pid == 0)
// 		{
// 			setup_pipefd(&fd, curr, true);
// 			exec_command_helper(curr, env);
// 		}
// 		setup_pipefd(&fd, curr, false);
// 		curr = curr->parent;
// 	}
// 	waitpid(pid, &status, 0);
// 	while (--cnt > 0)
// 		wait(NULL);
// 	if (WIFEXITED(status))
// 	{
// 		return ((unsigned char)WEXITSTATUS(status));
// 	}
// 	else
// 	{
// 		/*ここにシグナルの扱い*/
// 		return ((unsigned char)WEXITSTATUS(status));
// 	}
// }

// unsigned char node_expander(t_tree_node *node)
// {

// }