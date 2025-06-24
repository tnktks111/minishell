/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/24 18:33:58 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_and_or(t_tree_node *root, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid);
int				exec_pipeline(t_tree_node *root, t_env *env);
void			exec_command_helper(t_tree_node *cmd_node, t_env *env);

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
	env->envp = decode_table(env, false);
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
	int cnt;
	pid_t pid;

	cnt = 0;
	while (node->kind != NODE_PIPE_LINE)
	{
		if (node->parent->kind == NODE_PIPE && pipe(fd->pipefd) == -1)
			return (perror_string("pipe: "), -1);
		cnt++;
		if (prepare_here_doc(node, env) == EXIT_FAILURE)
			return (-1);
		pid = fork();
		if (node->parent->kind == NODE_PIPE_LINE)
			*lastpid = pid;
		if (pid == -1)
			return (perror_string("fork: "), -1);
		if (pid == 0)
		{
			setup_child_signal_handlers();
			setup_pipefd(fd, node, true);
			exec_command_helper(node, env);
		}
		setup_pipefd(fd, node, false);
		node = node->parent;
	}
	return (cnt);
}

/*fork, pipeのエラーハンドリングあとで*/
int	exec_pipeline(t_tree_node *node_pipeline, t_env *env)
{
	t_tree_node	*curr;
	pid_t		pid;
	int			cnt;
	t_pipefd	fd;
	int			status;

	expand_ast(node_pipeline, env);
	curr = node_pipeline->left;
	cnt = 0;
	fd.read_fd = STDIN_FILENO;
	if (curr->kind == NODE_SIMPLE_COMMAND || curr->kind == NODE_SUBSHELL)
		return (exec_solo_cmd(curr, env));
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	cnt = exec_loop(curr, &fd, env, &pid);
	if (cnt == -1)
		return (EXIT_FAILURE);
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	while (--cnt > 0)
		wait(NULL);
	setup_interactive_signal_handlers();
	unlink_all_tmpfiles(node_pipeline);
	return (status_handler(status));
}

void exec_command_helper(t_tree_node *node, t_env *env)
{
	t_tree_node	*cmd_node;

	if (node->right)
		cmd_node = node->right;
	else
		cmd_node = node;
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND)
	{
		if (!cmd_node->data.command.args[0] || ! cmd_node->data.command.args[0][0])
		{
			ft_puterr_general(cmd_node->data.command.args[0], "command not found");
			exit(127);
		}
		if (!ft_strchr(cmd_node->data.command.args[0], '/'))
		{
			find_builtin(cmd_node, env);
			find_path(cmd_node, env);
		}
		execve(cmd_node->data.command.args[0], cmd_node->data.command.args,
			env->envp);
		execve_failure_handler(cmd_node->data.command.args[0], errno);
	}
	else
	{
		exit(exec_ast(cmd_node, env));
	}
}
