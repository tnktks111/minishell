/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_solo_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/25 17:29:12 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	backup_stdin_out(int *stdin_out);
static void	restore_stdin_out(int *stdin_out);
static void exec_child_process_of_solo_cmd(t_tree_node *cmd_node, t_env *env);
int	exec_solo_cmd(t_tree_node *curr, t_env *env);

static void	backup_stdin_out(int *stdin_out)
{
	stdin_out[0] = dup(STDIN_FILENO);
	if (stdin_out[0] == -1)
	{
		perror("dup :");
		return ;
	}
	stdin_out[1] = dup(STDOUT_FILENO);
	if (stdin_out[1] == -1)
		perror("dup :");
}

static void	restore_stdin_out(int *stdin_out)
{
	dup2(stdin_out[0], STDIN_FILENO);
	dup2(stdin_out[1], STDOUT_FILENO);
}
static unsigned char exec_solo_builtin(t_tree_node *cmd_node, t_env *env)
{
    int stdin_out[2];
    unsigned char status;

    backup_stdin_out(stdin_out);
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	status = exec_builtin(cmd_node, env);
	restore_stdin_out(stdin_out);
	// unlink_tmpfile(cmd_node);
    return (status);
}

static void exec_child_process_of_solo_cmd(t_tree_node *cmd_node, t_env *env)
{
    setup_child_signal_handlers();
	if (exec_redirection(cmd_node->data.command.redirects) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	if (cmd_node->kind == NODE_SUBSHELL)
		exit(exec_ast(cmd_node, env));
	if (!cmd_node->data.command.args[0] || ! cmd_node->data.command.args[0][0])
	{
		ft_puterr_general(cmd_node->data.command.args[0], "command not found");
		exit(127);
	}
	if (!ft_strchr(cmd_node->data.command.args[0], '/'))
		find_path(cmd_node, env);
	execve(cmd_node->data.command.args[0], cmd_node->data.command.args, env->envp);
	execve_failure_handler(cmd_node->data.command.args[0], errno);
}

int	exec_solo_cmd(t_tree_node *cmd_node, t_env *env)
{
	pid_t			pid;
	int				wait_status;

	if (prepare_here_doc(cmd_node, env) == EXIT_FAILURE)
		return (env->prev_exit_status);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND)
		ft_set_underscore(cmd_node, env);
	if (cmd_node->kind == NODE_SIMPLE_COMMAND && is_builtin(cmd_node->data.command.args[0]))
		return (exec_solo_builtin(cmd_node, env));
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror_string("fork: "), EXIT_FAILURE);
		if (pid == 0)
        	exec_child_process_of_solo_cmd(cmd_node, env);
		setup_parent_wait_signal_handlers();
		wait(&wait_status);
		setup_interactive_signal_handlers();
		// unlink_tmpfile(cmd_node);
		return (status_handler(wait_status));
	}
}
