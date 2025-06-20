/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttanaka_minishell.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/20 18:52:00 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

void setup_interactive_signal_handlers(void);
void setup_parent_wait_signal_handlers(void);
void setup_child_signal_handlers(void);

bool have_quotes(char *limiter);
void remove_quotes(char **limiter);
void here_doc_expander(char **s, t_env *env);