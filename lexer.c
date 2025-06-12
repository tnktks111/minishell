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

bool	is_splitable(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<'
		|| c == '>');
}

bool	is_two_word_splitable(char c1, char c2)
{
	if (c1 == '<' && c2 == '<')
		return (true);
	else if (c1 == '>' && c2 == '>')
		return (true);
	else if (c1 == '|' && c2 == '|')
		return (true);
	else if (c1 == '&' && c2 == '&')
		return (true);
	else
		return (false);
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
		if (str[i] = 39)
		{
			if (!singquote)
				singquote = true;
			else
				singquote = false;
		}
		if (str[i] = 34)
		{
			if (!dubquote)
				dubquote = true;
			else
				dubquote = false;
		}
		if (is_splitable(str[i]))
		{
		}
		else if (str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
		{
			// space / tab / \n / '|' / '<''>'
			//'<<''>>' / '&&' '||'
		}
		else
		{
		}
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