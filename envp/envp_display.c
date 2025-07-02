/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:01:43 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 11:24:43 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	display_env(t_env *env, bool print_declare);

void	display_env(t_env *env, bool print_declare)
{
	size_t	i;
	char	**env_tab;

	env_tab = decode_table(env, print_declare);
	if (print_declare == true)
		ft_sort_string_tab(env_tab, ft_strcmp);
	i = 0;
	while (env_tab[i])
	{
		if (print_declare == true && ft_strncmp(env_tab[i], "_=", 2) == 0)
		{
			i++;
			continue ;
		}
		if (print_declare == true)
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env_tab[i++], STDOUT_FILENO);
	}
	free_splitted_data(env_tab);
	return ;
}
