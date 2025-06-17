/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/17 19:27:04 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

/* lexer prototype */
// utils
bool			is_d_quote(char c);
bool			is_s_quote(char c);
bool			is_splitable(char c);
bool			is_two_word_splitable(char c1, char c2);
char			*ft_strdup_len(char *src, size_t len);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
// token utils
t_token			*new_token(char *start, size_t len);
void			append_token(t_token **head, t_token *new);
size_t			token_strlen(char *str);
size_t			in_d_quote_token_strlen(char *str);
size_t			in_s_quote_token_strlen(char *str);
// token append
size_t			append_token_and_move_index(t_token **head, char *str,
					bool d_quote, bool s_quote);
size_t			append_splitable(t_token **head, char *str);
size_t			append_two_word_splitable(t_token **head, char *str);
t_token			*tokenize_str(char *str);
// void		print_tokens(t_token *head);
t_token			*lexer(char *str, t_env *env);

/* parser prototype */
// && ||
t_tree_node		*create_operator_node(t_token *op, t_tree_node *left,
					t_tree_node *right);
// | "PIPELINE"
t_tree_node		*create_pipeline_node(t_tree_node *pipe_root);
t_tree_node		*create_pipeline_tree(t_token *head, t_token *tail);
// redirect cmd

char			*ft_strchr(const char *s, int c);
int				ft_atoi(const char *nptr);
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

/* parser */

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
	if (ft_strncmp(redirect_str, ">>", 2) == 0)
		return (REDIR_APPEND);
	else if (ft_strncmp(redirect_str, "<<", 2) == 0)
		return (REDIR_HEREDOC);
	else if (ft_strncmp(redirect_str, "<", 2) == 0)
		return (REDIR_IN);
	else if (ft_strncmp(redirect_str, ">", 2) == 0)
		return (REDIR_OUT);
	else
		return (REDIR_IN);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		result;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] == ' ') || ((nptr[i] >= 9) && (nptr[i] <= 13)))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while ((nptr[i] >= '0') && (nptr[i] <= '9'))
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * result);
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

	cur = redirect_token->next;
	while (cur && cur->status == SPLITABLE)
		cur = cur->next;
	if (!cur || cur->status != NORMAL)
	{
		// syntax_error();
		return (NULL);
	}
	cur->status = USED;
	return (cur->str);
}

char	*ft_strchr(const char *s, int c)
{
	size_t			i;
	unsigned char	char_c;

	i = 0;
	char_c = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == char_c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
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
//	t_token		*output_name;
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
			append_redirects(&redirect_head, cur);
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
	cmd_node->parent = NULL;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->data.command.redirects = extract_redirects(head, tail);
	cmd_node->data.command.args = extract_args(head, tail);
	return (cmd_node);
}

t_tree_node	*create_pipeline_node(t_tree_node *cur_root)
{
	t_tree_node	*node;

	node = malloc(sizeof(t_tree_node));
	if (!node)
		return (NULL);
	node->kind = NODE_PIPE_LINE;
	node->parent = NULL;
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
		return (create_simple_cmd_node(head, tail));
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
		return (create_pipeline_node(paratheneses_root));
	}
	op = find_logical_operator(head, tail);
	if (!op)
	{
		pipeline_root = create_pipeline_tree(head, tail);
		return (create_pipeline_node(pipeline_root));
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

// void	print_indent(int level)
// {
// 	for (int i = 0; i < level; i++)
// 		printf("  ");
// }

// void	print_node_kind(t_node_kind kind)
// {
// 	if (kind == NODE_SIMPLE_COMMAND)
// 		printf("NODE_SIMPLE_COMMAND\n");
// 	else if (kind == NODE_SUBSHELL)
// 		printf("NODE_SUBSHELL\n");
// 	else if (kind == NODE_PIPE_LINE)
// 		printf("NODE_PIPE_LINE\n");
// 	else if (kind == NODE_PIPE)
// 		printf("NODE_PIPE '|'\n");
// 	else if (kind == NODE_AND)
// 		printf("NODE_AND '&&'\n");
// 	else if (kind == NODE_OR)
// 		printf("NODE_OR '||'\n");
// 	else if (kind == NODE_ROOT)
// 		printf("NODE_ROOT\n");
// 	else
// 		printf("UNKNOWN_NODE\n");
// }

// void	print_redirect_info(t_redirect *redirects, int indent)
// {
// 	t_redirect	*cur;

// 	cur = redirects;
// 	while (cur)
// 	{
// 		print_indent(indent);
// 		printf("Redirect: ");
// 		printf("io[%d] ", cur->io_number);
// 		if (cur->kind == REDIR_IN)
// 			printf("< ");
// 		else if (cur->kind == REDIR_OUT)
// 			printf("> ");
// 		else if (cur->kind == REDIR_APPEND)
// 			printf(">> ");
// 		else if (cur->kind == REDIR_HEREDOC)
// 			printf("<< ");
// 		if (cur->is_expandable)
// 			printf("filename(expandable): %s\n", cur->filename);
// 		else
// 			printf("filename: %s\n", cur->filename);
// 		cur = cur->next;
// 	}
// }

// void	print_args(char **args, int level)
// {
// 	if (!args)
// 		return ;
// 	for (int i = 0; args[i]; i++)
// 	{
// 		print_indent(level);
// 		printf("arg[%d]: %s\n", i, args[i]);
// 	}
// }

// void	print_tree(t_tree_node *node)
// {
// 	static int	level = 0;

// 	if (!node)
// 		return ;
// 	print_indent(level);
// 	print_node_kind(node->kind);
// 	if (node->kind == NODE_SIMPLE_COMMAND)
// 	{
// 		print_args(node->data.command.args, level + 1);
// 		if (node->data.command.redirects)
// 			print_redirect_info(node->data.command.redirects, level + 1);
// 	}
// 	level++;
// 	// PIPE_LINEノードは left しか持たない♥️
// 	if (node->kind == NODE_PIPE_LINE)
// 	{
// 		print_tree(node->left);
// 	}
// 	else
// 	{
// 		print_tree(node->left);
// 		print_tree(node->right);
// 	}
// 	level--;
// }

t_tree_node	*parser(t_token *head, t_env *env)
{
	t_tree_node	*root;
	t_token		*tail;

	tail = get_tail(head);
	root = create_tree(head, tail);
	root = add_tree_root(root);
	// print_tree(root);
	free_token(head, tail);
	exec_ast(root, env);
	return (root);
}

/* lexer */

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (!s1[i] || !s2[i])
			return (0);
		i++;
	}
	return (0);
}

bool	is_param(char c)
{
	return (c == '(' || c == ')');
}

bool	is_d_quote(char c)
{
	return (c == 34);
}

bool	is_s_quote(char c)
{
	return (c == 39);
}

bool	is_splitable(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<'
		|| c == '>');
}

bool	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

bool	is_valid_input(char *input)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		if (!is_space(input[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_two_word_splitable(char c1, char c2)
{
	if (c1 == '<' && c2 == '<')
		return (true);
	else if (c1 == '>' && c2 == '>')
		return (true);
	else if (c1 == '|' && c2 == '|')
		return (true);
	else if (c1 == '&' && c2 == '&')
		return (true);
	else
		return (false);
}

char	*ft_strdup_len(char *src, size_t len)
{
	char	*res;
	size_t	i;

	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

t_token	*new_token(char *start, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strdup_len(start, len);
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token *new)
{
	t_token	*last;

	if (!*head)
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

size_t	token_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && !is_splitable(str[len]) && !is_param(str[len])
		&& !(str[len + 1] && is_two_word_splitable(str[len], str[len + 1])))
		len++;
	return (len);
}

size_t	in_d_quote_token_strlen(char *str)
{
	size_t	len;

	len = 0;
	if (is_d_quote(str[len]))
		len++;
	while (str[len] && !is_d_quote(str[len]))
		len++;
	if (is_d_quote(str[len]))
		len++;
	return (len);
}

size_t	in_s_quote_token_strlen(char *str)
{
	size_t	len;

	len = 0;
	if (is_s_quote(str[len]))
		len++;
	while (str[len] && !is_s_quote(str[len]))
		len++;
	if (is_s_quote(str[len]))
		len++;
	return (len);
}

size_t	append_param_token_and_move_index(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 1;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strncmp(str, "(", 1) == 0)
		new->status = LEFT_PAREN;
	else if (ft_strncmp(str, ")", 1) == 0)
		new->status = RIGHT_PAREN;
	append_token(head, new);
	return (len);
}

size_t	append_token_and_move_index(t_token **head, char *str, bool d_quote,
		bool s_quote)
{
	size_t	len;
	t_token	*new;

	if (d_quote)
		len = in_d_quote_token_strlen(str);
	else if (s_quote)
		len = in_s_quote_token_strlen(str);
	else
		len = token_strlen(str);
	new = new_token(str, len);
	if (!new)
		return (0);
	if (d_quote)
		new->status = IN_DOUBLE;
	else if (s_quote)
		new->status = IN_SINGLE;
	else
		new->status = NORMAL;
	append_token(head, new);
	return (len);
}

size_t	append_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 1;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strncmp(new->str, "|", 2) == 0)
		new->status = PIPE;
	else if (ft_strncmp(new->str, "<", 2) == 0)
		new->status = REDIRECT;
	else if (ft_strncmp(new->str, ">", 2) == 0)
		new->status = REDIRECT;
	else
		new->status = SPLITABLE;
	append_token(head, new);
	return (len);
}

size_t	append_two_word_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 2;
	new = new_token(str, len);
	if (!new)
		return (0);
	if (ft_strncmp(new->str, "||", 3) == 0)
		new->status = AND_OR;
	else if (ft_strncmp(new->str, "&&", 3) == 0)
		new->status = AND_OR;
	else if (ft_strncmp(new->str, "<<", 3) == 0)
		new->status = REDIRECT;
	else if (ft_strncmp(new->str, ">>", 3) == 0)
		new->status = REDIRECT;
	else
		new->status = SPLITABLE;
	append_token(head, new);
	return (len);
}

t_token	*tokenize_str(char *str)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (str[i])
	{
		if (is_param(str[i]))
			i += append_param_token_and_move_index(&head, &str[i]);
		if (is_d_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], true, false);
		if (is_s_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], false, true);
		if (str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
			i += append_two_word_splitable(&head, &str[i]);
		else if (is_splitable(str[i]))
			i += append_splitable(&head, &str[i]);
		else
			i += append_token_and_move_index(&head, &str[i], false, false);
	}
	return (head);
}

// void	print_tokens(t_token *head)
// {
// 	while (head)
// 	{
// 		printf("Token: [%s]\n", head->str);
// 		if (head->status == IN_SINGLE)
// 			printf("IN_SINGLE \n");
// 		else if (head->status == IN_DOUBLE)
// 			printf("IN_DOUBLE \n");
// 		else if (head->status == LEFT_PAREN)
// 			printf("LEFT_PAREN \n");
// 		else if (head->status == RIGHT_PAREN)
// 			printf("RIGHT_PAREN \n");
// 		else if (head->status == AND_OR)
// 			printf("AND_OR \n");
// 		else if (head->status == REDIRECT)
// 			printf("REDIRECT \n");
// 		else if (head->status == SPLITABLE)
// 			printf("SPLITABLE \n");
// 		else if (head->status == PIPE)
// 			printf("PIPE \n");
// 		else if (head->status == NORMAL)
// 			printf("NORMAL \n");
// 		else if (head->status == USED)
// 			printf("USED \n");
// 		else
// 			printf("UNKNOWN (不明な状態: %u)\n", head->status);
// 		head = head->next;
// 	}
// }

t_token	*lexer(char *str, t_env *env)
{
	t_token	*head;

	head = tokenize_str(str);
	// print_tokens(head);
	parser(head, env);
	return (head);
}

/* expander */

// /////////////////////// \(^o^)/ ////////////////////////////////

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_env	env;
	
	(void)ac;
	(void)av;
	encode_envp(&env, envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (input == NULL)
		{
			printf("\nBye...\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
		}
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			printf("See you next time…\n");
			break ;
		}
		if (is_valid_input(input))
			lexer(input, &env);
		free(input);
	}
	return (0);
}

// /* */