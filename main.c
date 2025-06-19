/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:41:09 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/19 16:41:22 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_env	env;

	(void)ac;
	(void)av;

	rl_outstream = stderr;
	setup_interactive_signal_handlers();
	encode_envp(&env, envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (input == NULL)
		{
			ft_putendl_fd("exit\n", STDERR_FILENO);
			break ;
		}
		if (*input)
		{
			add_history(input);
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			ft_putendl_fd("See you next time…\n", STDERR_FILENO);
			break ;
		}
		if (is_valid_input(input))
		{
			lexer(input, &env);
		}
		free(input);
	}
	return (0);
}