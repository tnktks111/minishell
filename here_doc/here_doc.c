/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:35:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/24 19:23:09 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		prepare_here_doc(t_tree_node *node, t_env *env);
char	*here_doc_handler(t_redirect *redirect, t_env *env);

int	prepare_here_doc(t_tree_node *node, t_env *env)
{
	t_redirect	*curr;
	char		*tmpfile;

	if (node->right)
		curr = node->right->data.command.redirects;
	else
		curr = node->data.command.redirects;
	while (curr)
	{
		if (curr->kind == REDIR_HEREDOC)
		{
			tmpfile = here_doc_handler(curr, env);
			if (!tmpfile)
				return (EXIT_FAILURE);
			free(curr->filename);
			curr->filename = tmpfile;
		}
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}

char	*here_doc_handler(t_redirect *redirect, t_env *env)
{
	int		fd;
	int		original_stdin;
	char	*tmpfile;
	char	*s;
	bool	is_expandable;

	g_is_in_heredoc = 1;
	is_expandable = true;
	original_stdin = dup(STDIN_FILENO);
	if (have_quotes(redirect->filename) == true)
	{
		is_expandable = false;
		remove_quotes(redirect);
	}
	fd = sh_mktmpfd(&tmpfile);
	if (fd == -1)
		return (NULL);
	s = readline("> ");
	while (s)
	{
		if (ft_strcmp(s, redirect->filename) == 0)
		{
			free(s);
			break ;
		}
		if (is_expandable)
			here_doc_expander(&s, env);
		ft_putendl_fd(s, fd);
		free(s);
		s = readline("> ");
	}
	if (g_is_in_heredoc == 0)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	g_is_in_heredoc = 0;
	close(fd);
	return (tmpfile);
}
