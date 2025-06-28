/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 17:22:44 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	exec_ast(t_tree_node *root, t_env *env);
int				exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env,
					pid_t *lastpid);
int				exec_pipeline(t_tree_node *node_pipeline, t_env *env);
int				exec_pl_cmds(t_tree_node *node_pipeline, t_env *env);
void			exec_command_helper(t_tree_node *cmd_node, t_env *env);

unsigned char	exec_ast(t_tree_node *root, t_env *env)
{
	t_tree_node	*curr;
	int			prev_exit_status;

	curr = root->left;
	while (curr->kind == NODE_AND || curr->kind == NODE_OR)
		curr = curr->left;
	prev_exit_status = exec_pipeline(curr, env);
	if (prev_exit_status == HEREDOC_SIGINT)
	{
		env->prev_exit_status = 130;
		return (130);
	}
	curr = curr->parent;
	while ((prev_exit_status == 0 && curr->kind == NODE_AND)
		|| (curr->kind == NODE_OR))
	{
		if (curr->kind == NODE_OR && prev_exit_status == 0)
		{
			curr = curr->parent;
			continue;
		}
		prev_exit_status = exec_pipeline(curr->right, env);
		if (prev_exit_status == HEREDOC_SIGINT)
		{
			env->prev_exit_status = 130;
			return (130);
		}
		curr = curr->parent;
	}
	return (prev_exit_status);
}

int	exec_pipeline(t_tree_node *node_pipeline, t_env *env)
{
	if (env->envp_is_malloced == true)
		free_splited_data(env->envp);
	env->envp = decode_table(env, false);
	env->envp_is_malloced = true;
	node_pipeline->data.pipeline.exit_status = exec_pl_cmds(node_pipeline, env);
	if (node_pipeline->data.pipeline.exit_status == HEREDOC_SIGINT)
	{
		env->prev_exit_status = HEREDOC_SIGINT;
		return (HEREDOC_SIGINT);
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

int	exec_loop(t_tree_node *node, t_pipefd *fd, t_env *env, pid_t *lastpid)
{
	int		cnt;
	pid_t	pid;

	cnt = 0;
	while (node->kind != NODE_PIPE_LINE)
	{
		if (node->parent->kind == NODE_PIPE && pipe(fd->pipefd) == -1)
			return (perror_string("pipe: "), EXIT_FAILURE);
		if (prepare_here_doc(node, env) == EXIT_FAILURE)
			return (env->prev_exit_status);
		cnt++;
		pid = fork();
		if (pid == -1)
			return (perror_string("fork: "), EXIT_FAILURE);
		if (pid == 0)
		{
			setup_child_signal_handlers();
			setup_pipefd(fd, node, true);
			exec_command_helper(node, env);
		}
		*lastpid = pid;
		setup_pipefd(fd, node, false);
		node = node->parent;
	}
	return (cnt);
}

/*fork, pipeのエラーハンドリングあとで*/
int	exec_pl_cmds(t_tree_node *node_pipeline, t_env *env)
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
	if (cnt == EXIT_FAILURE || cnt == HEREDOC_SIGINT)
		return (cnt);
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	while (--cnt > 0)
		wait(NULL);
	setup_interactive_signal_handlers();
	unlink_all_tmpfiles(node_pipeline);
	return (status_handler(status));
}

void	exec_command_helper(t_tree_node *node, t_env *env)
{
	t_tree_node	*cmd_node;
	char		**args;

	env->is_child = true;
	if (node->right)
		cmd_node = node->right;
	else
		cmd_node = node;
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND)
	{
		args = cmd_node->data.command.args;
		if (!args[0] || !args[0][0])
		{
			ft_puterr_general(args[0], "command not found");
			exit(127);
		}
		find_builtin(cmd_node, env);
		find_path(cmd_node, env);
		execve(args[0], args, env->envp);
		execve_failure_handler(args[0], errno);
	}
	else
		exit(exec_ast(cmd_node, env));
}
