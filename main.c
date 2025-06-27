/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:41:09 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/27 17:06:02 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define RED "\1\033[31m\2"
#define BLUE "\1\033[32m\2"
#define RESET "\1\033[0m\2"

static bool	contain_space_only(char *s)
{
	while (*s)
		if (!(*s++ == ' '))
			return (false);
	return (true);
}

int	main(int ac, char **av, char **envp)
{
	char *input;
	t_env env;
	t_token *head;
	t_tree_node *root;

	(void)ac;
	(void)av;

	rl_outstream = stderr;
	setup_interactive_signal_handlers();
	encode_envp(&env, envp);
	env.envp_is_malloced = false;
	while (1)
	{
		input = readline(BLUE "minishell$ " RESET);
		if (input == NULL)
		{
			// free_table(&env);
			// ft_putendl_fd("exit", STDERR_FILENO);
			fprintf(stderr, RED "🂡🂡死神クイズに回答者はいらない♤\n" RESET);
			// ft_putendl_fd("Bye\n", STDERR_FILENO);
			free_all(&env);
			break ;
		}
		if (input[0] && !contain_space_only(input))
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			free_all(&env);
			ft_putendl_fd("コムギ...まだ居るか...\n", STDERR_FILENO);
			break ;
		}
		if (is_valid_input(input))
		{
			head = lexer(input);
			root = parser(head, &env);
			env.root = root;
			exec_ast(root, &env);
		}
		free(input);
	}
	return (0);
}