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

typedef struct s_command_line
{
	char					*arg;
	struct s_command_line	*next;
}							t_command_line;

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

void						append_command_line(t_command_line **head,
								char *str);

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
			// new_node = malloc(sizeof(t_command_line));
			// // if (!new_node)
			// // 	system_error();
			// new_node->arg = ft_strdup(files[i]);
			// new_node->next = NULL;
			// if (!*head)
			// 	*head = new_node;
			// else
			// 	tail->next = new_node;
			// tail = new_node;
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
	char		**expanded;
	char		*expanded_filename;

	cmd_args = NULL;
	simple_cmd_node = pipeline_node->left;
	if (simple_cmd_node->data.command.redirects)
	{
		redir_filename = simple_cmd_node->data.command.redirects->filename;
		is_filename_expandable = simple_cmd_node->data.command.redirects->is_expandable;
		if (is_filename_expandable && redir_filename)
		{
			expanded_filename = expand_every_variable(redir_filename, env);
			free(redir_filename);
			// if (check_wildcard_expand(redir_filename))
			// {
			// syntax_error();
			// }
			takeoff_quotes(expanded_filename);
			simple_cmd_node->data.command.redirects->filename = expanded_filename;
		}
	}
	if (simple_cmd_node->data.command.args)
	{
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
