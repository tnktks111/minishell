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

#include "../minishell.h"

void	get_cmd_line_list(t_list **head, char **cmd_args)
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
		// if (!new_token)
		// system_error();
		new_token->content = ft_strdup(cmd_args[i]);
		new_token->next = NULL;
		if (!*head)
			*head = new_token;
		else
			tail->next = new_token;
		tail = new_token;
		i++;
	}
}

void	expand_and_append_command_line(t_list **head, char *str, char **files)
{
	int		is_wildcard[PATH_MAX];
	size_t	i;
	bool	no_match;
	t_list	*tail;

	no_match = true;
	tail = *head;
	while (tail && tail->next)
		tail = tail->next;
	init_wildcard_array(is_wildcard, str, ft_strlen(str));
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

void	append_command_line(t_list **head, char *str)
{
	t_list	*new_node;
	t_list	*tail;

	new_node = malloc(sizeof(t_list));
	// if (!new_node)
	// 	system_error() ;
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
	// if (!result)
	// 	system_error();
	cur = head;
	while (i < count)
	{
		result[i++] = ft_strdup(cur->content);
		cur = cur->next;
	}
	result[i] = NULL;
	return (result);
}
