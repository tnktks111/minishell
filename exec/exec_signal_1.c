/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signal_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:27:44 by sguruge           #+#    #+#             */
/*   Updated: 2025/07/02 15:39:15 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		setup_interactive_signal_handlers(void);
void		setup_here_doc_signal_handlers(void);

static void	interactive_sigint_handler(int signo)
{
	(void)signo;
	g_rcv_sigint = 1;
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	here_doc_sigint_handler(int signo)
{
	(void)signo;
	g_rcv_sigint = 1;
	return ;
}

void	setup_interactive_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = interactive_sigint_handler;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_here_doc_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = here_doc_sigint_handler;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
