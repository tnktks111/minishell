/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:35:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/23 20:10:14 by ttanaka          ###   ########.fr       */
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
	char	*tmpfile;
	char	*s;
	bool	is_expandable;

	is_expandable = true;
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
	close(fd);
	return (tmpfile);
}
