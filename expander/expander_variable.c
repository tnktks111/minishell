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

	in_squote = false;
	in_dquote = false;
	i = 0;
	while (str[i])
	{
		if (!in_dquote && is_s_quote(str[i]))
			in_squote = !in_squote;
		if (!in_squote && is_d_quote(str[i]))
			in_dquote = !in_dquote;
		if (!in_squote && str[i] == '$' && str[i + 1])
			return (true);
		i++;
	}
	return (false);
}

char	*get_variable_str(char *src)
{
	size_t	i;
	size_t	start;
	size_t	len;

	len = 0;
	i = 0;
	while (src[i] != '$')
		i++;
	i++;
	start = i;
	if (src[i] == '?' || ft_isdigit(src[i]))
		return (ft_substr(src, start, 1));
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_'))
		i++;
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_'))
		i++;
	len = i - start;
	return (ft_substr(src, start, len));
}

char	*expand_individual_variable(char *str, t_env *env)
{
	char	*temp;
	char	*expanded_str;
	char	*variable;
	size_t	i;
	char	*before;
	char	*val;
	char	*after;

	i = 0;
	while (str[i] != '$')
		i++;
	variable = get_variable_str(&str[i]);
	val = ft_search(variable, env);
	if (!val)
		val = "";
	i = 0;
	while (str[i] != '$')
		i++;
	before = ft_substr(str, 0, i);
	after = ft_substr(&str[i + ft_strlen(variable) + 1], 0, ft_strlen(str));
	temp = ft_strjoin(before, val);
	expanded_str = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(variable);
	free(str);
	free(temp);
	return (expanded_str);
}

char	*expand_every_variable(char *str, t_env *env)
{
	while (check_variable_expand(str))
	{
		str = expand_individual_variable(str, env);
	}
	return (str);
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
