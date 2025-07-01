/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/01 17:40:59 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	expand_filename(t_redirect *cur, t_env *env)
{
	char	*redir_file;
	char	*temp;
	int		status;

	if (!cur->filename || cur->kind == REDIR_HEREDOC)
		return (EXIT_SUCCESS);
	redir_file = ft_strdup(cur->filename);
	temp = expand_every_variable(cur->filename, env);
	if (!temp || *temp == '\0')
	{
		free(temp);
		error_redir(redir_file);
		cur->filename = redir_file;
		return (ERROR_REDIR);
	}
	status = handle_file_wildcard(cur, temp, redir_file);
	if (status != EXIT_SUCCESS)
		return (status);
	free(redir_file);
	return (EXIT_SUCCESS);
}

void	expand_cmd_args(t_tree_node *simple_cmd_node, t_env *env)
{
	char	**cmd_args;
	char	**variable_expanded;
	char	**wildcard_expanded;
	size_t	i;

	wildcard_expanded = NULL;
	cmd_args = simple_cmd_node->data.command.args;
	variable_expanded = expand_cmd_variable(cmd_args, env);
	free_splited_data(cmd_args);
	simple_cmd_node->data.command.args = variable_expanded;
	if (variable_expanded[0])
	{
		wildcard_expanded = expand_cmd_wildcard(variable_expanded);
		i = 0;
		while (wildcard_expanded[i])
		{
			if (wildcard_expanded[i])
				takeoff_quotes(wildcard_expanded[i]);
			i++;
		}
		simple_cmd_node->data.command.args = wildcard_expanded;
	}
	else
		simple_cmd_node->data.command.args = ft_calloc(1, sizeof(char *));
	free_splited_data(variable_expanded);
}

int	expander(t_tree_node *simple_cmd_node, t_env *env)
{
	int			res_status;
	t_redirect	*cur;

	cur = NULL;
	if (simple_cmd_node->data.command.redirects)
		cur = simple_cmd_node->data.command.redirects;
	while (cur)
	{
		res_status = expand_filename(cur, env);
		if (res_status == EXIT_FAILURE)
			return (EXIT_FAILURE);
		else if (res_status == ERROR_REDIR)
			return (EXIT_FAILURE);
		cur = cur->next;
	}
	if (simple_cmd_node->data.command.args[0])
	{
		if (simple_cmd_node->data.command.args)
			expand_cmd_args(simple_cmd_node, env);
	}
	return (EXIT_SUCCESS);
}

int	expand_ast(t_tree_node *node, t_env *env)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->kind == NODE_SIMPLE_COMMAND || node->kind == NODE_SUBSHELL)
		return (expander(node, env));
	if (expand_ast(node->left, env) == EXIT_FAILURE || expand_ast(node->right,
			env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
