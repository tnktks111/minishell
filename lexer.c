/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/19 16:41:04 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* lexer prototype */
// utils
bool	is_d_quote(char c);
bool	is_s_quote(char c);
bool	is_splitable(char c);
bool	is_two_word_splitable(char c1, char c2);
char	*ft_strdup_len(char *src, size_t len);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
// token utils
t_token	*new_token(char *start, size_t len);
void	append_token(t_token **head, t_token *new);
size_t	token_strlen(char *str);
size_t	in_d_quote_token_strlen(char *str);
size_t	in_s_quote_token_strlen(char *str);
// token append
size_t	append_token_and_move_index(t_token **head, char *str, bool d_quote,
			bool s_quote);
size_t	append_splitable(t_token **head, char *str);
size_t	append_two_word_splitable(t_token **head, char *str);
t_token	*tokenize_str(char *str);
// void		print_tokens(t_token *head);
t_token	*lexer(char *str, t_env *env);

void	print_indent(int level)
{
	for (int i = 0; i < level; i++)
		printf("  ");
}

void	print_node_kind(t_node_kind kind)
{
	if (kind == NODE_SIMPLE_COMMAND)
		printf("NODE_SIMPLE_COMMAND\n");
	else if (kind == NODE_SUBSHELL)
		printf("NODE_SUBSHELL\n");
	else if (kind == NODE_PIPE_LINE)
		printf("NODE_PIPE_LINE\n");
	else if (kind == NODE_PIPE)
		printf("NODE_PIPE '|'\n");
	else if (kind == NODE_AND)
		printf("NODE_AND '&&'\n");
	else if (kind == NODE_OR)
		printf("NODE_OR '||'\n");
	else if (kind == NODE_ROOT)
		printf("NODE_ROOT\n");
	else
		printf("UNKNOWN_NODE\n");
}

void	print_redirect_info(t_redirect *redirects, int indent)
{
	t_redirect	*cur;

	cur = redirects;
	while (cur)
	{
		print_indent(indent);
		printf("Redirect: ");
		printf("io[%d] ", cur->io_number);
		if (cur->kind == REDIR_IN)
			printf("< ");
		else if (cur->kind == REDIR_OUT)
			printf("> ");
		else if (cur->kind == REDIR_APPEND)
			printf(">> ");
		else if (cur->kind == REDIR_HEREDOC)
			printf("<< ");
		if (cur->is_expandable)
			printf("filename(expandable): %s\n", cur->filename);
		else
			printf("filename: %s\n", cur->filename);
		cur = cur->next;
	}
}

void	print_args(char **args, int level)
{
	if (!args)
		return ;
	for (int i = 0; args[i]; i++)
	{
		print_indent(level);
		printf("arg[%d]: %s\n", i, args[i]);
	}
}

void	print_tree(t_tree_node *node)
{
	static int	level = 0;

	if (!node)
		return ;
	print_indent(level);
	print_node_kind(node->kind);
	if (node->kind == NODE_SIMPLE_COMMAND)
	{
		print_args(node->data.command.args, level + 1);
		if (node->data.command.redirects)
			print_redirect_info(node->data.command.redirects, level + 1);
	}
	level++;
	// PIPE_LINEノードは left しか持たない♥️
	if (node->kind == NODE_PIPE_LINE)
	{
		print_indent(level);
		printf("bang status:[%d]\n", node->data.pipeline.have_bang);
		print_tree(node->left);
	}
	else
	{
		print_tree(node->left);
		print_tree(node->right);
	}
	level--;
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
	if (ft_strcmp(new->str, "|") == 0)
		new->status = PIPE;
	else if (ft_strcmp(new->str, "<") == 0)
		new->status = REDIRECT;
	else if (ft_strcmp(new->str, ">") == 0)
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
		if (str[i] && str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
			i += append_two_word_splitable(&head, &str[i]);
		else if (is_splitable(str[i]))
			i += append_splitable(&head, &str[i]);
		else
			i += append_token_and_move_index(&head, &str[i], false, false);
	}
	return (head);
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: [%s]\n", head->str);
		if (head->status == IN_SINGLE)
			printf("IN_SINGLE \n");
		else if (head->status == IN_DOUBLE)
			printf("IN_DOUBLE \n");
		else if (head->status == LEFT_PAREN)
			printf("LEFT_PAREN \n");
		else if (head->status == RIGHT_PAREN)
			printf("RIGHT_PAREN \n");
		else if (head->status == AND_OR)
			printf("AND_OR \n");
		else if (head->status == REDIRECT)
			printf("REDIRECT \n");
		else if (head->status == SPLITABLE)
			printf("SPLITABLE \n");
		else if (head->status == PIPE)
			printf("PIPE \n");
		else if (head->status == NORMAL)
			printf("NORMAL \n");
		else if (head->status == USED)
			printf("USED \n");
		else
			printf("UNKNOWN (不明な状態: %u)\n", head->status);
		head = head->next;
	}
}

void	check_syntax_error(t_token *head)
{
	bool	prev_is_splitable;
	t_token	*cur;

	prev_is_splitable = false;
	cur = head;
	while (cur)
	{
		if (!prev_is_splitable && (cur->status == RIGHT_PAREN
				|| cur->status == REDIRECT || cur->status == AND_OR
				|| cur->status == PIPE))
			prev_is_splitable = true;
		if (prev_is_splitable && (cur->status == RIGHT_PAREN
				|| cur->status == REDIRECT || cur->status == AND_OR
				|| cur->status == PIPE))
			error_unexpected_token(cur->str);
		cur = cur->next;
	}
}

t_token	*lexer(char *str, t_env *env)
{
	t_token	*head;

	head = tokenize_str(str);
	// print_tokens(head);
	// check_syntax_error(head);
	parser(head, env);
	return (head);
}

/* expander */