/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_wildcard_utils1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-24 05:44:10 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-24 05:44:10 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	get_cmd_line_list(t_list **head, char **cmd_args)
{
	size_t	i;
	t_list	*new_token;
	t_list	*tail;

	*head = NULL;
	tail = NULL;
	i = 0;
	while (cmd_args[i])
	{
		new_token = malloc(sizeof(t_list));
		if (!new_token)
			return (ft_puterr_malloc(), EXIT_FAILURE);
		new_token->content = ft_strdup(cmd_args[i]);
		new_token->next = NULL;
		if (!*head)
			*head = new_token;
		else
			tail->next = new_token;
		tail = new_token;
		i++;
	}
	return (EXIT_SUCCESS);
}

int	expand_and_append_command_line(char *str, t_list **head)
{
	t_list	*expanded;
	t_list	*tail;

	expanded = NULL;
	ft_glob(str, &expanded);
	if (expanded)
	{
		if (!*head)
			*head = expanded;
		else
		{
			tail = *head;
			while (tail->next)
				tail = tail->next;
			tail->next = expanded;
		}
	}
	else
		return (append_command_line(head, str));
	return (EXIT_SUCCESS);
}

int	append_command_line(t_list **head, char *str)
{
	t_list	*new_node;
	t_list	*tail;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return (ft_puterr_malloc(), EXIT_FAILURE);
	new_node->content = ft_strdup(str);
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
	return (EXIT_SUCCESS);
}

char	**list_to_args(t_list *head)
{
	size_t	count;
	t_list	*cur;
	char	**result;
	size_t	i;

	count = 0;
	cur = head;
	i = 0;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (ft_puterr_malloc());
	cur = head;
	while (i < count)
	{
		result[i++] = ft_strdup(cur->content);
		if (!result)
			return (free_allocated_data(result, i), ft_puterr_malloc());
		cur = cur->next;
	}
	return (result[i] = NULL, result);
}

char	**expand_file_line(t_list *fileline)
{
	char	**result;
	t_list	*head;
	t_list	*cur;

	head = NULL;
	append_command_line(&head, fileline->content);
	cur = fileline;
	while (cur)
	{
		if (check_wildcard_expand(cur->content))
			expand_and_append_command_line(cur->content, &head);
		else
			append_command_line(&head, cur->content);
		cur = cur->next;
	}
	result = list_to_args(head);
	free_list(head);
	free_list(fileline);
	return (result);
}
