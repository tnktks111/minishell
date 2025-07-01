/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:57:22 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 19:53:07 by ttanaka          ###   ########.fr       */
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
	char	*redir_filename;
	char	*exp_filename;
	char	*temp;
	bool	error;

	error = false;
	redir_filename = ft_strdup(cur->filename);
	if (cur->filename && cur->kind != REDIR_HEREDOC)
	{
		temp = expand_every_variable(cur->filename, env);
		if (check_wildcard_expand(temp))
		{
			exp_filename = expand_filename_wildcard(temp, &error);
			free(temp);
			temp = exp_filename;
			if (!exp_filename && !error)
				return (EXIT_FAILURE);
			else if (error)
				return (cur->filename = redir_filename,
					error_ambiguous_redirect(redir_filename), ERROR_AMBIGUOS);
		}
		takeoff_quotes(temp);
		cur->filename = temp;
	}
	free(redir_filename);
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
		else if (res_status == ERROR_AMBIGUOS)
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
	if (node->kind == NODE_SIMPLE_COMMAND)
		return (expander(node, env));
	if (expand_ast(node->left, env) == EXIT_FAILURE || expand_ast(node->right,
			env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
}
