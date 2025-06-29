/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025/06/29 18:14:20 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	else if (node->kind == NODE_SUBSHELL)
	{
		print_indent(level);
		printf("(SUBSHELL)\n");
		if (node->data.command.redirects)
			print_redirect_info(node->data.command.redirects, level + 1);
		print_tree(node->left);
	}
	else
	{
		print_tree(node->left);
		print_tree(node->right);
	}
	level--;
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

// for debug

t_token	*tokenize_str(char *str)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] && str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
			i += append_two_word_splitable(&head, &str[i]);
		else if (is_param(str[i]))
			i += append_param_token_and_move_index(&head, &str[i]);
		else if (is_d_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], true, false);
		else if (is_s_quote(str[i]))
			i += append_token_and_move_index(&head, &str[i], false, true);
		else if (is_splitable(str[i]))
			i += append_splitable(&head, &str[i]);
		else
			i += append_token_and_move_index(&head, &str[i], false, false);
	}
	return (head);
}

t_token	*lexer(char *str)
{
	t_token	*head;

	head = tokenize_str(str);
	fix_non_splitted_tokens(&head);
	// print_tokens(head);
	return (head);
}
