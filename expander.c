/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/21 19:25:48 by ttanaka          ###   ########.fr       */
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

char	*get_variable_str(char *src)
{
	size_t	i;
	size_t	start;
	size_t	len;

	len = 0;
	i = 0;
	while (src[i] != '$')
		i++;
	i++;
	start = i;
	if (src[i] == '?' || ft_isdigit(src[i]))
		return (ft_substr(src, start, 1));
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_'))
		i++;
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_'))
		i++;
	len = i - start;
	return (ft_substr(src, start, len));
}

char	*expand_individual_variable(char *str, t_env *env)
{
	char	*temp;
	char	*expanded_str;
	char	*variable;
	size_t	i;
	char	*before;
	char	*val;
	char	*after;

	i = 0;
	while (str[i] != '$')
		i++;
	variable = get_variable_str(&str[i]);
	val = ft_search(variable, env);
	if (!val)
		val = "";
	i = 0;
	while (str[i] != '$')
		i++;
	before = ft_substr(str, 0, i);
	after = ft_substr(&str[i + ft_strlen(variable) + 1], 0, ft_strlen(str));
	temp = ft_strjoin(before, val);
	expanded_str = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(variable);
	free(str);
	free(temp);
	return (expanded_str);
}

char	*expand_every_variable(char *str, t_env *env)
{
	while (check_variable_expand(str))
	{
		str = expand_individual_variable(str, env);
	}
	return (str);
}

// void	wildcard_expand_str(char *str, t_env *env)
// {
// }

void	takeoff_quotes(char *str)
{
	size_t	read_index;
	size_t	write_index;
	bool	in_squote;
	bool	in_dquote;

	read_index = 0;
	write_index = 0;
	in_squote = false;
	in_dquote = false;
	if (!str)
		return ;
	while (str[read_index])
	{
		if (!in_dquote && is_s_quote(str[read_index]))
		{
			in_squote = !in_squote;
			read_index++;
		}
		else if (!in_squote && is_d_quote(str[read_index]))
		{
			in_dquote = !in_dquote;
			read_index++;
		}
		else
			str[write_index++] = str[read_index++];
	}
	str[write_index] = '\0';
}

void	expander(t_tree_node *pipeline_node, t_env *env)
{
	t_tree_node	*simple_cmd_node;
	char		**cmd_args;
	char		*redir_filename;
	bool		is_filename_expandable;
	size_t		i;

	simple_cmd_node = pipeline_node->left;
	if (simple_cmd_node->data.command.redirects)
	{
		redir_filename = simple_cmd_node->data.command.redirects->filename;
		is_filename_expandable = simple_cmd_node->data.command.redirects->is_expandable;
		if (is_filename_expandable && redir_filename)
		{
			simple_cmd_node->data.command.redirects->filename = expand_every_variable(redir_filename,
					env);
		}
		takeoff_quotes(redir_filename);
	}
	if (simple_cmd_node->data.command.args)
	{
		cmd_args = simple_cmd_node->data.command.args;
		i = 0;
		while (cmd_args[i])
		{
			if (check_variable_expand(cmd_args[i]))
				cmd_args[i] = expand_every_variable(cmd_args[i], env);
			takeoff_quotes(cmd_args[i]);
			i++;
		}
	}
	// if (is_filename_expandable)
	// 	redir_filename = expand_every_variable(redir_filename, env);
	// while (cmd_args[i])
	// {
	// 	if (check_wildcard_expand(cmd_args[i]))
	// 		wildcard_expand_str(cmd_args[i], env);
	// 	i++;
	// }
	// if (check_wildcard_expand(redir_filename))
	// 	syntax_error();
}

void	expand_ast(t_tree_node *node, t_env *env)
{
	if (!node)
		return ;
	if (node->kind == NODE_PIPE_LINE)
		expander(node, env);
	expand_ast(node->left, env);
	expand_ast(node->right, env);
}
