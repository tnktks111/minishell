/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/27 17:26:23 by ttanaka          ###   ########.fr       */
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

void	expand_filename(t_tree_node *simple_cmd_node, t_env *env)
{
	char	*redir_filename;
	char	*expanded_filename;
	char	*temp;
	char	**files;

	files = gen_tmp_dir_file_array();
	redir_filename = simple_cmd_node->data.command.redirects->filename;
	if (redir_filename)
	{
		temp = expand_every_variable(redir_filename, env);
		if (check_wildcard_expand(temp))
		{
			expanded_filename = expand_filename_wildcard(temp, files);
			free(temp);
			temp = expanded_filename;
		}
		if (simple_cmd_node->data.command.redirects->kind != REDIR_HEREDOC)
			takeoff_quotes(temp);
		simple_cmd_node->data.command.redirects->filename = temp;
	}
	free_splited_data(files);
}

void	expand_cmd_args(t_tree_node *simple_cmd_node, t_env *env)
{
	char	**cmd_args;
	size_t	i;
	char	**expanded;

	cmd_args = simple_cmd_node->data.command.args;
	i = 0;
	while (cmd_args[i])
	{
		if (check_variable_expand(cmd_args[i]))
		{
			cmd_args[i] = expand_every_variable(cmd_args[i], env);
		}
		i++;
	}
	expanded = expand_cmd_wildcard(cmd_args);
	free_splited_data(cmd_args);
	i = 0;
	while (expanded[i])
	{
		takeoff_quotes(expanded[i]);
		i++;
	}
	simple_cmd_node->data.command.args = expanded;
}

void	expander(t_tree_node *pipeline_node, t_env *env)
{
	t_tree_node	*simple_cmd_node;

	simple_cmd_node = pipeline_node->left;
	if (simple_cmd_node->data.command.redirects)
		expand_filename(simple_cmd_node, env);
	if (simple_cmd_node->data.command.args[0])
	{
		if (simple_cmd_node->data.command.args)
			expand_cmd_args(simple_cmd_node, env);
	}
}

void	expand_ast(t_tree_node *node, t_env *env)
{
	if (!node)
		return ;
	if (node->kind == NODE_PIPE_LINE)
	{
		expander(node, env);
	}
	expand_ast(node->left, env);
	expand_ast(node->right, env);
}
