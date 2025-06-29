/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-23 13:29:59 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-23 13:29:59 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	if (!head || !tail)
		return (NULL);
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

// t_redirect	*cur;
// if (cmd_node->data.command.redirects)
// 	cur = cmd_node->data.command.redirects;
// if (cur)
// {
// 	printf("%s\n", cur->filename);
// 	cur = cur->next;
// }