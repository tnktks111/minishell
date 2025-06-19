/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttanaka_minishell.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:17 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/19 15:18:55 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

void setup_interactive_signal_handlers(void);
void setup_parent_wait_signal_handlers(void);
void setup_child_signal_handlers(void);