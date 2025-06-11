/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguruge <sguruge@student.42tokyo.jp>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-11 12:49:45 by sguruge           #+#    #+#             */
/*   Updated: 2025-06-11 12:49:45 by sguruge          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <readline/history.h>
#include <readline/readline.h>

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline(">>minishell ");
		if (input == NULL)
		{
			printf("\nBye...\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
		}
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			printf("See you next time…\n");
			break ;
		}
		printf("You said: %s\n", input);
		free(input);
	}
	return (0);
}

t_token	*tokenize_str(char *str)
{
	t_token	*head;
	size_t	i;
	bool	singquote;
	bool	dubquote;

	head = NULL;
	singquote = false;
	dubquote = false;
	i = 0;
	while (str[i])
	{
		if (str[i] = "'")
		{
			if (!singquote)
				singquote = true;
			else
				singquote = false;
		}
		if (str[i] = '"')
		{
			if (!dubquote)
				dubquote = true;
			else
				dubquote = false;
		}
		if (is_splitable(str[i])) 
		{
			// space / tab / \n / '|' / '<''>'
		}
		else if (str[i + 1] && is_twoword_splitable(str[i], str[i + 1]))
		{
			//'<<''>>' / '&&' '||'
		}
		else
			i++;
	}
	return (head);
}

t_token	**lexer(char *str)
{
	t_token	*head;

	head = tokenize_str(str);
	return (head);
}

/* */