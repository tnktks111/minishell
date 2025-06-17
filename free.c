/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:03:28 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/17 15:03:48 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_splited_data(char **data);
char **free_allocated_data(char **datas, size_t allocated);

void	free_splited_data(char **data)
{
	size_t	i;

	i = -1;
	while (data[++i])
		free(data[i]);
	free(data);
}

char **free_allocated_data(char **datas, size_t allocated)
{
    size_t i;

    i = 0;
    while (i < allocated)
        free(datas[i++]);
    free(datas);
    return (NULL);
}