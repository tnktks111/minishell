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

bool	check_variable_expand(char *str)
{
	bool	in_squote;
	bool	in_dquote;
	size_t	i;

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (str[i])
	{
		if (!in_dquote && is_s_quote(str[i]))
			in_squote = !in_squote;
		if (!in_squote && is_d_quote(str[i]))
			in_dquote = !in_dquote;
		if (!in_squote && str[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

bool	check_wildcard_expand(char *str)
{
	bool	in_squote;
	bool	in_dquote;
	size_t	i;

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (str[i])
	{
		if (!in_dquote && is_s_quote(str[i]))
			in_squote = !in_squote;
		if (!in_squote && is_d_quote(str[i]))
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote && str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

void	variable_expand_str(char *str, t_env *env)
{
	char	*temp;
	char	*to_expand;
}

void	wildcard_expand_str(char *str, t_env *env)
{
}

void	expander(t_tree_node *pipeline_node, t_env *env)
{
	t_tree_node	*simple_cmd_node;
	char		**cmd_args;
	char		redir_filename;
	bool		is_filename_expandable;
	size_t		i;

	simple_cmd_node = pipeline_node->left;
	cmd_args = simple_cmd_node->data.command.args;
	redir_filename = simple_cmd_node->data.command.redirects->filename;
	is_filename_expandable = simple_cmd_node->data.command.redirects->is_expandable;
	i = 0;
	while (cmd_args[i])
	{
		if (check_variable_expand(cmd_args[i]))
			variable_expand_str(cmd_args[i], env);
		i++;
	}
	if (is_filename_expandable)
		variable_expand_str(redir_filename, env);
	while (cmd_args[i])
	{
		if (check_wildcard_expand(cmd_args[i]))
			wildcard_expand_str(cmd_args[i], env);
		i++;
	}
	// if (check_wildcard_expand(redir_filename))
	// 	syntax_error();
}
