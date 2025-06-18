/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guruge_minishell.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-18 09:00:44 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-18 09:00:44 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GURUGE_MINISHELL_H
# define GURUGE_MINISHELL_H

# include "libft/inc/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define DEFAULT_TMPDIR "."
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif
# define RANDOM_SIZE 512
# define HASH_SIZE 1024

#endif
