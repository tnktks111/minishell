/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-02 06:35:25 by sguruge           #+#    #+#             */
/*   Updated: 2025-07-02 06:35:25 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*expand_file_variable(char *str, t_env *env, bool *redir_error)
{
	char	*result;
	char	*init_expand;
	char	**splitted_expansion;

	init_expand = str;
	while (check_variable_expand(init_expand))
	{
		init_expand = expand_individual_variable(init_expand, env);
	}
	result = ft_strdup(init_expand);
	splitted_expansion = ft_split_isspace(init_expand);
	free(init_expand);
	if (count_args(splitted_expansion) > 1)
		*redir_error = true;
	free_splitted_data(splitted_expansion);
	return (result);
}

bool	check_if_splittable(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			return (true);
		i++;
	}
	return (false);
}

int	variable_expand_append_command_line(char *str, t_list **head, t_env *env)
{
	char	*expanded_str;
	char	**splitted;
	size_t	i;

	i = 0;
	expanded_str = expand_every_variable(ft_strdup(str), env);
	if (!expanded_str)
		return (EXIT_FAILURE);
	if (expanded_str && expanded_str[0] != '\0'
		&& !check_if_splittable(expanded_str))
		append_command_line(head, expanded_str);
	else if (check_if_splittable(expanded_str))
	{
		splitted = ft_split_isspace(expanded_str);
		while (splitted[i])
		{
			append_command_line(head, splitted[i]);
			i++;
		}
		free_splitted_data(splitted);
	}
	free(expanded_str);
	return (EXIT_SUCCESS);
}

char	**expand_cmd_variable(char **cmd_args, t_env *env)
{
	t_list	*cmd_line;
	t_list	*cur;
	char	**result;
	t_list	*head;

	head = NULL;
	get_cmd_line_list(&cmd_line, cmd_args);
	cur = cmd_line;
	while (cur)
	{
		if (check_variable_expand(cur->content))
			variable_expand_append_command_line(cur->content, &head, env);
		else
			append_command_line(&head, cur->content);
		cur = cur->next;
	}
	result = list_to_args(head);
	free_list(head);
	free_list(cmd_line);
	return (result);
}
