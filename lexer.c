/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-11 12:49:45 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

bool		is_d_quote(char c);
bool		is_s_quote(char c);
bool		is_splitable(char c);
bool		is_two_word_splitable(char c1, char c2);
char		*ft_strdup(char *src, size_t len);
t_token		*new_token(char *start, size_t len);
void		append_token(t_token **head, t_token *new);
size_t		token_strlen(char *str);
size_t		in_d_quote_token_strlen(char *str);
size_t		in_s_quote_token_strlen(char *str);
size_t		append_token_and_move_index(t_token **head, char *str, bool d_quote,
				bool s_quote);
size_t		append_splitable(t_token **head, char *str);
size_t		append_two_word_splitable(t_token **head, char *str);
t_token		*tokenize_str(char *str);
void		print_tokens(t_token *head);
t_token		*lexer(char *str);

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

char	*ft_strdup(char *src, size_t len)
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
	token->str = ft_strdup(start, len);
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
	while (str[len] && !is_splitable(str[len]) && !(str[len + 1]
			&& is_two_word_splitable(str[len], str[len + 1])))
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
	else if (!s_quote)
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
		if (is_d_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], true, false);
		else if (is_s_quote(str[i]))
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

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: [%s]\n", head->str);
		printf("Status: [%u]\n", head->status);
		head = head->next;
	}
}

t_token	*lexer(char *str)
{
	t_token	*head;

	head = tokenize_str(str);
	print_tokens(head);
	return (head);
}

/* expander */

/* parser */

t_tree_node	*init_tree_root(void);
t_tree_node	*create_tree(t_token *head, t_token *tail);
t_tree_node	*parser(t_token *head);

t_tree_node	*init_tree_root(void)
{
	t_tree_node	*root;

	root = malloc(sizeof(t_tree_node));
	if (!root)
		return (NULL);
	root->kind = NODE_ROOT;
	root->parent = NULL;
	root->left = NULL;
	root->right = NULL;
	root->data.pipeline.have_bang = false;
	root->data.pipeline.exit_status = 0;
	return (root);
}

t_token	*find_lowest_precedence_operator(t_token *head, t_token *tail)
{
	t_token	*op;
	size_t	i;

	i = 0;
	if (head->status == SPLITABLE) // '<<' '<' '||' '|' ...
	{
		op = head;
		return (op);
	}
	else if (head->status == IN_DOUBLE || head->status == IN_SINGLE) // IN_QUOTE
		return (NULL);
	else // NORMAL cmd
		return (NULL);
}

t_tree_node	*create_tree(t_token *head, t_token *tail)
{
	t_tree_node	*root;
	t_token		*op;
	t_tree_node	*left;
	t_tree_node	*right;

	if (!head || !tail)
		return (NULL);
	root = init_tree_root();
	op = find_lowest_precedence_operator(head, tail);
	if (op)
	{
		root->kind = get_node_kind(op);
		left = create_tree(head, op->prev);
		right = create_tree(op->next, tail);
	}
	else
		create_simple_cmd_node(root, head, tail);
	return (root);
}

t_tree_node	*parser(t_token *head)
{
	t_tree_node	*root;
	t_token		*tail;

	tail = get_tail(head);
	root = create_tree(head, tail);
	print_tree(root);
	free_token(head);
	return (root);
}

////////////////////////\(^o^)/////////////////////////////////

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline(">>minishell ");
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
		lexer(input);
		free(input);
	}
	return (0);
}

/* */