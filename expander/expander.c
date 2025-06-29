/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 12:39:30 by ttanaka          ###   ########.fr       */
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

int	expand_filename(t_tree_node *simple_cmd_node, t_env *env)
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
			if (!expanded_filename)
				return (EXIT_FAILURE);
		}
		if (simple_cmd_node->data.command.redirects->kind != REDIR_HEREDOC)
			takeoff_quotes(temp);
		simple_cmd_node->data.command.redirects->filename = temp;
	}
	free_splited_data(files);
	return (EXIT_SUCCESS);
}

void	expand_cmd_args(t_tree_node *simple_cmd_node, t_env *env)
{
	char	**cmd_args;
	char	**variable_expanded;
	size_t	i;
	char	**wildcard_expanded;

	cmd_args = simple_cmd_node->data.command.args;
	variable_expanded = expand_cmd_variable(cmd_args, env);
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

int	expander(t_tree_node *simple_cmd_node, t_env *env)
{
	int	res_status;

	if (simple_cmd_node->data.command.redirects)
	{
		res_status = expand_filename(simple_cmd_node, env);
		if (res_status == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (simple_cmd_node->data.command.args[0])
	{
		if (simple_cmd_node->data.command.args)
			expand_cmd_args(simple_cmd_node, env);
	}
	return (EXIT_SUCCESS);
}

void	expand_ast(t_tree_node *node, t_env *env)
{
	if (!node)
		return ;
	if (node->kind == NODE_SIMPLE_COMMAND)
	{
		expander(node, env);
	}
	expand_ast(node->left, env);
	expand_ast(node->right, env);
}
