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

void	get_cmd_line_list(t_command_line **head, char **cmd_args)
{
	size_t			i;
	t_command_line	*new_token;
	t_command_line	*tail;

	*head = NULL;
	tail = NULL;
	i = 0;
	while (cmd_args[i])
	{
		new_token = malloc(sizeof(t_command_line));
		// if (!new_token)
		// system_error();
		new_token->arg = ft_strdup(cmd_args[i]);
		new_token->next = NULL;
		if (!*head)
			*head = new_token;
		else
			tail->next = new_token;
		tail = new_token;
		i++;
	}
}

void	expand_and_append_command_line(t_command_line **head, char *str,
		char **files)
{
	int				is_wildcard[PATH_MAX];
	size_t			i;
	bool			no_match;
	t_command_line	*tail;

	no_match = true;
	i = 0;
	tail = *head;
	while (tail && tail->next)
		tail = tail->next;
	while (i < ft_strlen(str))
	{
		if (str[i] == '*')
			is_wildcard[i] = 1;
		else
			is_wildcard[i] = 0;
		i++;
	}
	i = 0;
	while (files[i])
	{
		if (ft_ismatch(files[i], str, is_wildcard, ft_strlen(str)))
		{
			append_command_line(head, files[i]);
			no_match = false;
		}
		i++;
	}
	if (no_match)
		append_command_line(head, str);
}

void	append_command_line(t_command_line **head, char *str)
{
	t_command_line	*new_node;
	t_command_line	*tail;

	new_node = malloc(sizeof(t_command_line));
	// if (!new_node)
	// 	system_error() ;
	new_node->arg = ft_strdup(str);
	new_node->next = NULL;
	if (!*head)
		*head = new_node;
	else
	{
		tail = *head;
		while (tail->next)
			tail = tail->next;
		tail->next = new_node;
	}
}

char	**list_to_args(t_command_line *head)
{
	size_t			count;
	t_command_line	*cur;
	char			**result;
	size_t			i;

	count = 0;
	cur = head;
	i = 0;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	result = malloc(sizeof(char *) * (count + 1));
	// if (!result)
	// 	system_error();
	cur = head;
	while (i < count)
	{
		result[i++] = ft_strdup(cur->arg);
		cur = cur->next;
	}
	result[i] = NULL;
	return (result);
}

char	**expand_cmd_line(t_command_line *cmdline)
{
	char			**all_files;
	char			**result;
	t_command_line	*head;
	t_command_line	*cur;

	head = NULL;
	all_files = gen_tmp_dir_file_array();
	cur = cmdline;
	append_command_line(&head, cur->arg);
	cur = cur->next;
	while (cur)
	{
		if (check_wildcard_expand(cur->arg))
			expand_and_append_command_line(&head, cur->arg, all_files);
		else
			append_command_line(&head, cur->arg);
		cur = cur->next;
	}
	result = list_to_args(head);
	// free_cmd_line(head);
	return (result);
}

char	**expand_every_wildcard(char **cmd_args)
{
	t_command_line	*cmd_line;
	char			**result;

	get_cmd_line_list(&cmd_line, cmd_args);
	result = expand_cmd_line(cmd_line);
	// free_args(cmd_args);
	// free_cmd_line(cmd_line);
	return (result);
}

char	*expand_filename_wildcard(char *filename)
{
	int		is_wildcard[PATH_MAX];
	size_t	i;
	int		matches;
	char	**files;
	char	*result;

	result = NULL;
	files = gen_tmp_dir_file_array();
	matches = 0;
	i = 0;
	while (i < ft_strlen(filename))
	{
		if (filename[i] == '*')
			is_wildcard[i] = 1;
		else
			is_wildcard[i] = 0;
		i++;
	}
	i = 0;
	while (files[i])
	{
		if (ft_ismatch(files[i], filename, is_wildcard, ft_strlen(filename)))
		{
			if (matches == 0)
				result = ft_strdup(files[i]);
			matches++;
		}
		i++;
	}
	if (matches > 1)
	{
		error_ambiguous_redirect(filename);
		return (NULL);
	}
	if (matches == 0)
		return (ft_strdup(filename));
	return (result);
}

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

	redir_filename = simple_cmd_node->data.command.redirects->filename;
	if (redir_filename)
	{
		temp = expand_every_variable(redir_filename, env);
		if (check_wildcard_expand(temp))
		{
			expanded_filename = expand_filename_wildcard(temp);
			free(temp);
			temp = expanded_filename;
		}
		takeoff_quotes(temp);
		simple_cmd_node->data.command.redirects->filename = temp;
	}
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
			cmd_args[i] = expand_every_variable(cmd_args[i], env);
		i++;
	}
	expanded = expand_every_wildcard(cmd_args);
	// free_args(cmd_args);
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
	if (simple_cmd_node->data.command.args)
		expand_cmd_args(simple_cmd_node, env);
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
