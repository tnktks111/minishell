/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 14:08:39 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_pipeline(t_tree_node *node_pipeline, t_env *env);
int				exec_pl_cmds(t_tree_node *node_pipeline, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env,
					pid_t *lastpid);
void			exec_command_helper(t_tree_node *cmd_node, t_env *env);

unsigned char	exec_ast(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;
	int			prev_exit_status;

	curr = root->left;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
		curr = curr->left;
	prev_exit_status = exec_pipeline(curr, env);
	if (prev_exit_status == -1)
		return (env->prev_exit_status = 130, free_for_exec_ast(env));
	curr = curr->parent;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
	{
		if ((prev_exit_status == 0 && curr->kind == NODE_AND)
			|| (prev_exit_status && curr->kind == NODE_OR))
			prev_exit_status = exec_pipeline(curr->right, env);
		curr = curr->parent;
	}
	return (free_for_exec_ast(env));
}

int	exec_pipeline(t_tree_node *node_pipeline, t_env *env)
{
	if (env->envp_is_malloced == true)
		free_splitted_data(env->envp);
	env->envp = decode_table(env, false);
	env->envp_is_malloced = true;
	node_pipeline->data.pipeline.exit_status = exec_pl_cmds(node_pipeline, env);
	if (node_pipeline->data.pipeline.exit_status == -1)
	{
		env->prev_exit_status = 130;
		return (-1);
	}
	if (node_pipeline->data.pipeline.have_bang == true)
	{
		env->prev_exit_status = !node_pipeline->data.pipeline.exit_status;
		return (!node_pipeline->data.pipeline.exit_status);
	}
	else
	{
		env->prev_exit_status = node_pipeline->data.pipeline.exit_status;
		return (node_pipeline->data.pipeline.exit_status);
	}
}

int	exec_pl_cmds(t_tree_node *node_pipeline, t_env *env)
{
	t_tree_node	*curr;
	pid_t		pid;
	int			cnt;
	t_pipefd	fd;
	int			status;

	if (expand_ast(node_pipeline, env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	curr = node_pipeline->left;
	fd.read_fd = STDIN_FILENO;
	if (curr->kind == NODE_SIMPLE_COMMAND || curr->kind == NODE_SUBSHELL)
		return (exec_solo_cmd(curr, env));
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	cnt = exec_loop(curr, &fd, env, &pid);
	if (cnt == EXIT_FAILURE)
		return (EXIT_FAILURE);
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	n_wait(cnt - 1);
	setup_interactive_signal_handlers();
	return (status_handler(status));
}

int	exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid)
{
	int		cnt;
	pid_t	pid;

	cnt = 0;
	while (node->kind != NODE_PIPE_LINE)
	{
		if (node->parent->kind == NODE_PIPE && pipe(fd->pipefd) == -1)
			return (perror_string("pipe: "), EXIT_FAILURE);
		pid = fork();
		if (pid == -1)
			return (perror_string("fork: "), EXIT_FAILURE);
		if (pid == 0)
		{
			setup_child_signal_handlers();
			setup_pipefd(fd, node, true);
			exec_command_helper(node, env);
		}
		cnt++;
		*lastpid = pid;
		setup_pipefd(fd, node, false);
		node = node->parent;
	}
	return (cnt);
}

void	exec_command_helper(t_tree_node *node, t_env *env)
{
	char	**args;

	env->is_child = true;
	if (node->right)
		node = node->right;
	if (exec_redirection(node->data.command.redirects) == EXIT_FAILURE)
		free_for_exit(env, EXIT_FAILURE);
	if (node->kind == NODE_SIMPLE_COMMAND)
	{
		args = node->data.command.args;
		if (!args[0])
			free_for_exit(env, 0);
		if (!args[0][0])
		{
			ft_puterr_general(args[0], "command not found");
			free_for_exit(env, 127);
		}
		find_builtin(node, env);
		find_path(node->data.command.args, env);
		execve(args[0], args, env->envp);
		execve_failure_handler(args[0], errno, env);
	}
	else
		exit(exec_ast(node, env));
}
