/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-08 09:49:58 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-08 09:49:58 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>

typedef enum e_status
{
	IN_SINGLE,
	IN_DOUBLE,
	NORMAL
}					t_status;

typedef struct s_token
{
	char			*str;
	bool			is_meta;
	t_status		status;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

#endif
