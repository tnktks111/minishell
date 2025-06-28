/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_wildcard.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-24 05:15:08 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-24 05:15:08 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**expand_cmd_line(t_list *cmdline)
{
	char	**result;
	t_list	*head;
	t_list	*cur;

	head = NULL;
	append_command_line(&head, cmdline->content);
	cur = cmdline->next;
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
	free_list(cmdline);
	return (result);
}

char	**expand_cmd_wildcard(char **cmd_args)
{
	t_list	*cmd_line;
	char	**result;

	get_cmd_line_list(&cmd_line, cmd_args);
	result = expand_cmd_line(cmd_line);
	return (result);
}

void	init_wildcard_array(int *src, char *str, size_t str_len)
{
	size_t	i;

	i = 0;
	while (i < str_len)
	{
		if (str[i] == '*')
			src[i] = 1;
		else
			src[i] = 0;
		i++;
	}
}

char	*expand_filename_wildcard(char *filename, char **files)
{
	int		is_wildcard[PATH_MAX];
	size_t	i;
	int		matches;
	char	*result;

	result = NULL;
	init_wildcard_array(is_wildcard, filename, ft_strlen(filename));
	matches = 0;
	i = 0;
	while (files[i])
	{
		if (ft_ismatch(files[i], filename, is_wildcard, ft_strlen(filename)))
		{
			if (matches == 0)
				result = ft_strdup(files[i]);
			matches++;
		}
		i++;
	}
	if (matches > 1)
	{
		error_ambiguous_redirect(filename);
		return (NULL);
	}
	if (matches == 0)
		return (ft_strdup(filename));
	return (result);
}
