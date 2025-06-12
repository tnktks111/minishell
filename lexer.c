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

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

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

char	*ft_strdup(char *src, size_t len)
{
	char	*res;
	size_t	i;

	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

t_token	*new_token(char *start, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strdup(start, len);
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token *new)
{
	t_token	*last;

	if (!*head)
	{
		*head = new;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

size_t	get_token_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (str[len] && !is_splitable(str[len]) && !(str[len + 1]
			&& is_two_word_splitable(str[len], str[len + 1])))
		len++;
	return (len);
}

size_t	append_token_and_move_index(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = get_token_strlen(str);
	new = new_token(str, len);
	if (!new)
		return (0);
	append_token(head, new);
	return (len);
}

size_t	append_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 1;
	new = new_token(str, len);
	if (!new)
		return (0);
	append_token(head, new);
	return (len);
}

size_t	append_two_word_splitable(t_token **head, char *str)
{
	size_t	len;
	t_token	*new;

	len = 2;
	new = new_token(str, len);
	if (!new)
		return (0);
	append_token(head, new);
	return (len);
}

t_token	*tokenize_str(char *str)
{
	t_token	*head;
	bool	d_quote;
	size_t	i;

	d_quote = false;
	head = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == 34)
			d_quote = true;
		if (str[i + 1] && is_two_word_splitable(str[i], str[i + 1]))
			i += append_two_word_splitable(&head, &str[i]);
		else if (is_splitable(str[i]))
			i += append_splitable(&head, &str[i]);
		else
			i += append_token_and_move_index(&head, &str[i]);
	}
	return (head);
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: [%s]\n", head->str);
		head = head->next;
	}
}

t_token	*lexer(char *str)
{
	t_token	*head;

	head = tokenize_str(str);
	print_tokens(head);
	return (head);
}

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
		lexer(input);
		free(input);
	}
	return (0);
}

/* */