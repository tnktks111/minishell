/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-24 05:42:45 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-24 05:42:45 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_variable_expand(char *str)
{
	bool	in_squote;
	bool	in_dquote;
	size_t	i;
	size_t	j;

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (str[i])
	{
		if (!in_dquote && is_s_quote(str[i]))
			in_squote = !in_squote;
		if (!in_squote && is_d_quote(str[i]))
			in_dquote = !in_dquote;
		j = i + 1;
		if (!in_squote && str[i] == '$' && str[i + 1] && (ft_isalnum(str[j])
				|| str[j] == '_' || str[j] == '?' || is_s_quote(str[j])))
			return (true);
		i++;
	}
	return (false);
}

size_t	count_args(char **args)
{
	size_t	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

char	**expand_cmd_variable(char **cmd_args, t_env *env)
{
	char	**new_args;
	size_t	i;
	size_t	j;
	char	*expanded;

	new_args = malloc(sizeof(char *) * (count_args(cmd_args) + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd_args[i])
	{
		if (check_variable_expand(cmd_args[i]))
			expanded = expand_every_variable(cmd_args[i], env);
		else
			expanded = ft_strdup(cmd_args[i]);
		if (expanded && expanded[0] != '\0')
			new_args[j++] = expanded;
		else
			free(expanded);
		i++;
	}
	new_args[j] = NULL;
	return (new_args);
}
