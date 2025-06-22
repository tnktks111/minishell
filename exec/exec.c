/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/22 20:02:46 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_and_or(t_tree_node *root, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid);
int				exec_pipeline(t_tree_node *root, t_env *env);
unsigned char	exec_command_helper(t_tree_node *cmd_node, t_env *env);

unsigned char	exec_ast(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;
	int			prev_exit_status;

	curr = root->left;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
		curr = curr->left;
	prev_exit_status = exec_and_or(curr, env);
	if (prev_exit_status == -1)
	{
		env->prev_exit_status = 130;
		return (130);
	}
	curr = curr->parent;
	while ((!prev_exit_status && curr->kind == NODE_AND) || (prev_exit_status
			&& curr->kind == NODE_OR))
	{
		prev_exit_status = exec_and_or(curr->right, env);
		if (prev_exit_status == -1)
		{
			env->prev_exit_status = 130;
			return (130);
		}
		curr = curr->parent;
	}
	return (prev_exit_status);
}

int	exec_and_or(t_tree_node *root, t_env *env)
{
	env->envp = decode_table(env);
	root->data.pipeline.exit_status = exec_pipeline(root, env);
	if (root->data.pipeline.have_bang == true)
	{
		env->prev_exit_status = !root->data.pipeline.exit_status;
		return (!root->data.pipeline.exit_status);
	}
	else
	{
		env->prev_exit_status = root->data.pipeline.exit_status;
		return (root->data.pipeline.exit_status);
	}
}

int exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid)
{
	size_t cnt;
	pid_t pid;

	cnt = 0;
	while (node->kind != NODE_PIPE_LINE)
	{
		if (node->parent->kind == NODE_PIPE && pipe(fd->pipefd) == -1)
			return (perror_string("pipe: "));
		cnt++;
		prepare_here_doc(node, env);
		pid = fork();
		if (node->parent->kind == NODE_PIPE_LINE)
			*lastpid = pid;
		if (pid == -1)
			return (perror_string("fork: "));
		if (pid == 0)
		{
			setup_child_signal_handlers();
			setup_pipefd(fd, node, true);
			exec_command_helper(node, env);
		}
		setup_pipefd(fd, node, false);
		node = node->parent;
	}
	return (EXIT_SUCCESS);
}

/*fork, pipeのエラーハンドリングあとで*/
int	exec_pipeline(t_tree_node *node_pipeline, t_env *env)
{
	t_tree_node	*curr;
	pid_t		pid;
	size_t		cnt;
	t_pipefd	fd;
	int			status;

	expand_ast(node_pipeline, env);
	curr = node_pipeline->left;
	cnt = 0;
	fd.read_fd = STDIN_FILENO;
	if (curr->kind == NODE_SIMPLE_COMMAND)
		return (exec_solo_cmd(curr, env));
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	if (exec_loop(curr, &fd, env, &pid) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	while (--cnt > 0)
		wait(NULL);
	setup_interactive_signal_handlers();
	unlink_all_tmpfiles(node_pipeline);
	return (status_handler(status));
}

unsigned char	exec_command_helper(t_tree_node *node, t_env *env)
{
	t_tree_node	*cmd_node;

	if (node->right)
		cmd_node = node->right;
	else
		cmd_node = node;
	exec_redirection(cmd_node->data.command.redirects);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND)
	{
		if (!ft_strchr(cmd_node->data.command.args[0], '/'))
		{
			find_builtin(cmd_node, env);
			find_path(cmd_node, env);
		}
		execve(cmd_node->data.command.args[0], cmd_node->data.command.args,
			env->envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		exit(exec_ast(cmd_node, env));
	}
}
