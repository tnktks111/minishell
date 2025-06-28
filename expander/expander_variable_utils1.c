/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable_utils1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-28 09:28:37 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-28 09:28:37 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*create_expanded_str(char *str, char *val, char *variable, size_t i)
{
	char	*temp;
	char	*expanded_str;
	char	*before;
	char	*after;

	before = ft_substr(str, 0, i);
	after = ft_substr(&str[i + ft_strlen(variable) + 1], 0, ft_strlen(str));
	temp = ft_strjoin(before, val);
	expanded_str = ft_strjoin(temp, after);
	free(temp);
	free(before);
	free(after);
	return (expanded_str);
}

char	*expand_individual_variable(char *str, t_env *env)
{
	char	*expanded_str;
	char	*variable;
	size_t	i;
	char	*val;

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
	expanded_str = create_expanded_str(str, val, variable, i);
	free(variable);
	free(str);
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
