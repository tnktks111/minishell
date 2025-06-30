/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:03:28 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/30 12:02:51 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tree_node(t_tree_node *node);
void	free_redirects(t_redirect *redirect);
void	free_list(t_list *list);
void	free_splited_data(char **data);
char	**free_allocated_data(char **datas, size_t allocated);

void	free_token(t_token *head, t_token *tail)
{
	t_token	*cur;
	t_token	*next;

	{
		if (!head)
			return ;
		cur = head;
		while (cur)
		{
			next = cur->next;
			if (cur->str)
				free(cur->str);
			free(cur);
			if (cur == tail)
				break ;
			cur = next;
		}
	}
}

void	free_list(t_list *list)
{
	t_list	*cur;
	t_list	*next;

	{
		if (!list)
			return ;
		cur = list;
		while (cur)
		{
			next = cur->next;
			if (cur->content)
				free(cur->content);
			free(cur);
			cur = next;
		}
	}
}

void	free_splited_data(char **data)
{
	size_t	i;

	i = -1;
	while (data[++i])
		free(data[i]);
	free(data);
}

char	**free_allocated_data(char **datas, size_t allocated)
{
	size_t	i;

	i = 0;
	while (i < allocated)
		free(datas[i++]);
	free(datas);
	return (NULL);
}

void	free_redirects(t_redirect *redirect)
{
	t_redirect	*tmp;

	while (redirect)
	{
		tmp = redirect;
		redirect = redirect->next;
		free(tmp->filename);
		free(tmp);
	}
}

void	free_tree_node(t_tree_node *node)
{
	if (!node)
		return ;
	free_tree_node(node->left);
	free_tree_node(node->right);
	if (node->kind == NODE_SIMPLE_COMMAND || node->kind == NODE_SUBSHELL)
	{
		if (node->data.command.redirects)
			free_redirects(node->data.command.redirects);
	}
	free(node);
}
