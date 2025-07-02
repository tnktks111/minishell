/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:41:09 by ttanaka           #+#    #+#             */
/*   Updated: 2025/07/02 15:44:17 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

#define RED "\1\033[31m\2"
#define GREEN "\1\033[32m\2"
#define RESET "\1\033[0m\2"

volatile sig_atomic_t	g_rcv_sigint;

static int	init_minishell(t_env *env, char **envp)
{
	rl_outstream = stderr;
	setup_interactive_signal_handlers();
	if (encode_envp(env, envp) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	env->envp_is_malloced = false;
	env->is_child = false;
	env->prev_exit_status = 0;
	return (EXIT_SUCCESS);
}

static void	line_executer(char *input, t_env *env)
{
	t_token		*head;
	t_tree_node	*root;

	head = lexer(input);
	root = parser(head, env);
	if (root)
	{
		env->root = root;
		if (prepare_entire_here_docs(root, env) == EXIT_FAILURE)
		{
			env->prev_exit_status = 130;
			free_tree_node(env->root);
			return ;
		}
		exec_ast(root, env);
	}
}

static void interactive_sigint_rcver(t_env *env)
{
	env->prev_exit_status = 130;
	g_rcv_sigint = 0;
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_env	env;

	(void)ac;
	(void)av;
	g_rcv_sigint = 0;
	if (init_minishell(&env, envp) == EXIT_FAILURE)
		return (1);
	while (1)
	{
		input = readline(GREEN "minishell$ " RESET);
		if (g_rcv_sigint == 1)
			interactive_sigint_rcver(&env);
		if (input == NULL)
			break ;
		if (input[0])
			add_history(input);
		if (is_valid_input(input, &env))
			line_executer(input, &env);
		free(input);
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	free_table(&env);
	return (env.prev_exit_status);
}
