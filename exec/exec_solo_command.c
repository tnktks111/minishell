/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_solo_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 01:15:05 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static unsigned char	exec_solo_builtin(t_tree_node *cmd_node, t_env *env);
static void				exec_child_process_of_solo_cmd(t_tree_node *cmd_node,
							t_env *env);
int						exec_solo_cmd(t_tree_node *curr, t_env *env);

static unsigned char	exec_solo_builtin(t_tree_node *cmd_node, t_env *env)
{
	int				saved_std_fds[3];
	unsigned char	status;

	if (save_std_fds(saved_std_fds) == EXIT_FAILURE)
	{
		unlink_tmpfile(cmd_node);
		return (EXIT_FAILURE);
	}
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
	{
		unlink_tmpfile(cmd_node);
		restore_std_fds(saved_std_fds);
		return (EXIT_FAILURE);
	}
	status = exec_builtin(cmd_node, env);
	restore_std_fds(saved_std_fds);
	unlink_tmpfile(cmd_node);
	return (status);
}

static void	exec_child_process_of_solo_cmd(t_tree_node *cmd_node, t_env *env)
{
	env->is_child = true;
	setup_child_signal_handlers();
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
		free_for_exit(env, EXIT_FAILURE);
	if (cmd_node->kind == NODE_SUBSHELL)
		exit(exec_ast(cmd_node, env, true));
	if (!cmd_node->data.command.args || !cmd_node->data.command.args[0])
		free_for_exit(env, 0);
	if (!cmd_node->data.command.args[0][0])
	{
		ft_puterr_general(cmd_node->data.command.args[0], "command not found");
		free_for_exit(env, 127);
	}
	if (!ft_strchr(cmd_node->data.command.args[0], '/'))
		find_path(cmd_node->data.command.args, env);
	execve(cmd_node->data.command.args[0], cmd_node->data.command.args,
		env->envp);
	execve_failure_handler(cmd_node->data.command.args[0], errno, env);
}

int	exec_solo_cmd(t_tree_node *cmd_node, t_env *env)
{
	pid_t	pid;
	int		wait_status;

	if (prepare_here_doc(cmd_node, env) == EXIT_FAILURE)
		return (env->prev_exit_status);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND && cmd_node->data.command.args[0]
		&& ft_set_underscore(cmd_node, env) == EXIT_FAILURE)
		return (unlink_tmpfile(cmd_node), EXIT_FAILURE);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND && cmd_node->data.command.args
		&& is_builtin(cmd_node->data.command.args[0]))
		return (exec_solo_builtin(cmd_node, env));
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror_string("fork"), unlink_tmpfile(cmd_node),
				EXIT_FAILURE);
		if (pid == 0)
			exec_child_process_of_solo_cmd(cmd_node, env);
		setup_parent_wait_signal_handlers();
		wait(&wait_status);
		setup_interactive_signal_handlers();
		unlink_tmpfile(cmd_node);
		return (status_handler(wait_status));
	}
}
