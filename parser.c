/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 09:35:10 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/21 16:16:17 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parser prototype */
// && ||
t_tree_node		*create_operator_node(t_token *op, t_tree_node *left,
					t_tree_node *right);
// | "PIPELINE"
t_tree_node		*create_pipeline_node(t_tree_node *pipe_root, t_token *head,
					t_token *tail);
t_tree_node		*create_pipeline_tree(t_token *head, t_token *tail);
// redirect cmd

char			**extract_args(t_token *head, t_token *tail);
t_redirect_kind	get_redirect_kind(t_token *redirect_token);
int				get_io_number(t_redirect_kind kind, t_token *redirect_token);
char			*get_output_name(t_token *redirect_token);
bool			check_if_expandable(char *filename);
void			append_redirects(t_redirect **head, t_token *redirect_token);
t_redirect		*extract_redirects(t_token *head, t_token *tail);
t_tree_node		*create_simple_cmd_node(t_token *head, t_token *tail);

t_tree_node		*create_tree(t_token *head, t_token *tail);
// utils
t_node_kind		get_node_kind(t_token *token);
t_token			*find_third_lowest_precedence_operator(t_token *head,
					t_token *tail);
t_token			*find_second_lowest_precedence_operator(t_token *head,
					t_token *tail);
t_token			*find_lowest_precedence_operator(t_token *head, t_token *tail);
t_token			*find_logical_operator(t_token *head, t_token *tail);
// paser main
t_token			*get_tail(t_token *head);
t_tree_node		*add_tree_root(t_tree_node *root);
void			free_token(t_token *head, t_token *tail);
t_tree_node		*parser(t_token *head, t_env *env);

t_tree_node	*add_tree_root(t_tree_node *root)
{
	t_tree_node	*new_root;

	new_root = malloc(sizeof(t_tree_node));
	if (!new_root)
		return (NULL);
	new_root->kind = NODE_ROOT;
	new_root->parent = NULL;
	new_root->left = root;
	new_root->right = NULL;
	new_root->data.pipeline.have_bang = false;
	new_root->data.pipeline.exit_status = 0;
	if (root)
		root->parent = new_root;
	return (new_root);
}

t_node_kind	get_node_kind(t_token *token)
{
	if (ft_strncmp(token->str, "||", 3) == 0)
		return (NODE_OR);
	else if (ft_strncmp(token->str, "&&", 3) == 0)
		return (NODE_AND);
	else if (ft_strncmp(token->str, "|", 2) == 0)
		return (NODE_PIPE);
	else
		return (NODE_SIMPLE_COMMAND);
}

t_token	*find_third_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == PIPE)
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

t_token	*find_second_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_AND)
			return (cur);
		cur = cur->prev;
	}
	return (find_third_lowest_precedence_operator(head, tail));
}

t_token	*find_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_OR)
			return (cur);
		cur = cur->prev;
	}
	return (find_second_lowest_precedence_operator(head, tail));
}

t_token	*find_logical_operator(t_token *head, t_token *tail)
{
	t_token	*cur;

	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_OR)
			return (cur);
		cur = cur->prev;
	}
	cur = tail;
	while (cur && cur != head->prev)
	{
		if (cur->status == AND_OR && get_node_kind(cur) == NODE_AND)
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

char	**extract_args(t_token *head, t_token *tail)
{
	char	**cmd_args;
	size_t	count;
	t_token	*cur;

	count = 0;
	cur = head;
	while (cur && cur != tail->next && cur->status != PIPE)
	{
		if (cur->status != SPLITABLE && cur->status != USED)
			count++;
		cur = cur->next;
	}
	cmd_args = malloc(sizeof(char *) * (count + 1));
	if (!cmd_args)
		return (NULL);
	cur = head;
	count = 0;
	while (cur && cur != tail->next && cur->status != PIPE)
	{
		if (cur->status != SPLITABLE && cur->status != USED)
			cmd_args[count++] = ft_strdup_len(cur->str, ft_strlen(cur->str));
		cur = cur->next;
	}
	cmd_args[count] = NULL;
	return (cmd_args);
}

t_redirect_kind	get_redirect_kind(t_token *redirect_token)
{
	char	*redirect_str;

	redirect_str = redirect_token->str;
	if (ft_strcmp(redirect_str, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(redirect_str, "<<") == 0)
		return (REDIR_HEREDOC);
	else if (ft_strcmp(redirect_str, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(redirect_str, ">") == 0)
		return (REDIR_OUT);
	else
		return (REDIR_IN);
}

int	get_io_number(t_redirect_kind kind, t_token *redirect_token)
{
	t_token	*prev;

	prev = redirect_token->prev;
	if (!prev || prev->status != NORMAL)
	{
		if (kind == REDIR_IN || kind == REDIR_HEREDOC)
			return (0);
		else if (kind == REDIR_OUT || kind == REDIR_APPEND)
			return (1);
		else
			return (1);
	}
	prev->status = USED;
	return (ft_atoi(prev->str));
}

char	*get_output_name(t_token *redirect_token)
{
	t_token	*cur;
	char	*file_name;

	cur = redirect_token->next;
	while (cur && cur->status == SPLITABLE)
		cur = cur->next;
	if (!cur || cur->status != NORMAL)
	{
		// syntax_error();
		return (NULL);
	}
	cur->status = USED;
	file_name = ft_strdup(cur->str);
	return (file_name);
}

bool	check_if_expandable(char *filename)
{
	size_t	len;

	if (!filename)
		return (false);
	len = ft_strlen(filename);
	if (len >= 2 && is_s_quote(filename[0]) && is_s_quote(filename[len - 1]))
		return (false);
	return (ft_strchr(filename, '$') != NULL);
}

void	append_redirects(t_redirect **head, t_token *redirect_token)
{
	t_redirect	*new;
	t_redirect	*cur;

	if (!redirect_token || !redirect_token->next)
		return ;
	new = malloc(sizeof(t_redirect));
	if (!new)
		return ;
	new->kind = get_redirect_kind(redirect_token);
	new->io_number = get_io_number(new->kind, redirect_token);
	new->filename = get_output_name(redirect_token);
	new->is_expandable = check_if_expandable(new->filename);
	new->next = NULL;
	redirect_token->status = USED;
	if (*head == NULL)
		*head = new;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

t_redirect	*extract_redirects(t_token *head, t_token *tail)
{
	t_redirect	*redirect_head;
	t_token		*cur;

	redirect_head = NULL;
	cur = head;
	while (cur && cur != tail->next && cur->status != PIPE)
	{
		if (cur->status == REDIRECT)
		{
			append_redirects(&redirect_head, cur);
		}
		cur = cur->next;
	}
	return (redirect_head);
}

t_tree_node	*create_simple_cmd_node(t_token *head, t_token *tail)
{
	t_tree_node	*cmd_node;

	cmd_node = malloc(sizeof(t_tree_node));
	if (!cmd_node)
		return (NULL);
	cmd_node->kind = NODE_SIMPLE_COMMAND;
	find_bang(head, tail);
	cmd_node->parent = NULL;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->data.command.redirects = extract_redirects(head, tail);
	cmd_node->data.command.args = extract_args(head, tail);
	return (cmd_node);
}

bool	find_bang(t_token *head, t_token *tail)
{
	t_token	*cur;
	bool	result;

	result = false;
	cur = head;
	while (cur && cur != tail->next)
	{
		cur = skip_splitable_forward(cur);
		if (!cur)
			break ;
		if (cur->status == LEFT_PAREN)
			break ;
		if (ft_strncmp(cur->str, "!", 2) == 0)
		{
			result = !result;
			cur->status = USED;
			cur = cur->next;
		}
		else
			break ;
	}
	return (result);
}

t_tree_node	*create_pipeline_node(t_tree_node *cur_root, t_token *head,
		t_token *tail)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_PIPE_LINE;
	node->parent = NULL;
	node->data.pipeline.have_bang = find_bang(head, tail);
	node->left = cur_root;
	node->right = NULL;
	if (cur_root)
		cur_root->parent = node;
	return (node);
}

t_tree_node	*create_pipeline_tree(t_token *head, t_token *tail)
{
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;

	op = find_third_lowest_precedence_operator(head, tail);
	if (!op)
	{
		return (create_simple_cmd_node(head, tail));
	}
	left = create_pipeline_tree(head, op->prev);
	right = create_pipeline_tree(op->next, tail);
	return (create_operator_node(op, left, right));
}

t_tree_node	*create_subshell_node(t_tree_node *cur_root)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_SUBSHELL;
	node->left = cur_root;
	node->right = NULL;
	node->parent = NULL;
	if (cur_root)
		cur_root->parent = node;
	return (node);
}

bool	is_parentheses_group(t_token *head, t_token *tail)
{
	int		paren_level;
	t_token	*cur;

	paren_level = 0;
	cur = head;
	if (!head || !tail)
		return (false);
	if (head->status != LEFT_PAREN || tail->status != RIGHT_PAREN)
		return (false);
	while (cur && cur != tail->next)
	{
		if (cur->status == LEFT_PAREN)
			paren_level++;
		else if (cur->status == RIGHT_PAREN)
			paren_level--;
		if (paren_level == 0 && cur != tail)
			return (false);
		cur = cur->next;
	}
	return (paren_level == 0);
}

t_tree_node	*create_operator_node(t_token *op, t_tree_node *left,
		t_tree_node *right)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = get_node_kind(op);
	node->parent = NULL;
	node->left = left;
	node->right = right;
	if (left)
		left->parent = node;
	if (right)
		right->parent = node;
	return (node);
}

t_token	*skip_splitable_forward(t_token *token)
{
	while (token && token->status == SPLITABLE)
		token = token->next;
	return (token);
}

t_token	*skip_splitable_backward(t_token *token)
{
	while (token && token->status == SPLITABLE)
		token = token->prev;
	return (token);
}

t_tree_node	*create_tree(t_token *head, t_token *tail)
{
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;
	t_tree_node	*pipeline_root;
	t_tree_node	*paratheneses_root;

	if (!head || !tail)
		return (NULL);
	head = skip_splitable_forward(head);
	tail = skip_splitable_backward(tail);
	if (is_parentheses_group(head, tail))
	{
		pipeline_root = create_tree(head->next, tail->prev);
		paratheneses_root = create_subshell_node(pipeline_root);
		return (create_pipeline_node(paratheneses_root, head, tail));
	}
	op = find_logical_operator(head, tail);
	if (!op)
	{
		pipeline_root = create_pipeline_tree(head, tail);
		return (create_pipeline_node(pipeline_root, head, tail));
	}
	left = create_tree(head, op->prev);
	right = create_tree(op->next, tail);
	return (create_operator_node(op, left, right));
}

t_token	*get_tail(t_token *head)
{
	t_token	*tail;
	t_token	*last_valid;

	tail = head;
	last_valid = head;
	while (tail)
	{
		if (tail->str && tail->str[0] != '\0')
			last_valid = tail;
		tail = tail->next;
	}
	return (last_valid);
}

void	free_token(t_token *head, t_token *tail)
{
	t_token	*cur;
	t_token	*next;

	cur = head;
	while (cur && cur != tail->next)
	{
		next = cur->next;
		if (cur->str)
			free(cur->str);
		free(cur);
		cur = next;
	}
}

void			expand_ast(t_tree_node *node, t_env *env);

t_tree_node	*parser(t_token *head, t_env *env)
{
	t_tree_node	*root;
	t_token		*tail;

	tail = get_tail(head);
	root = create_tree(head, tail);
	root = add_tree_root(root);
	free_token(head, tail);
	print_tree(root);
	/*expandのタイミングを後で修正する*/
	expand_ast(root, env);
	exec_ast(root, env);
	return (root);
}
