/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:35:53 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/26 18:37:52 by ttanaka          ###   ########.fr       */
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
	bool	end_by_delimiter;
	int 	status;
	pid_t	pid;

	is_expandable = true;
	end_by_delimiter = false;
	if (have_quotes(redirect->filename) == true)
	{
		is_expandable = false;
		remove_quotes(redirect);
	}
	fd = sh_mktmpfd(&tmpfile);
	if (fd == -1)
	{
		env->prev_exit_status = EXIT_FAILURE;
		return (NULL);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork:");
		env->prev_exit_status = EXIT_FAILURE;
		return (NULL);
	}
	if (pid == 0)
	{
		setup_here_doc_signal_handlers();
		s = readline("> ");
		while (s)
		{
			if (ft_strcmp(s, redirect->filename) == 0)
			{
				free(s);
				end_by_delimiter = true;
				break ;
			}
			if (is_expandable)
				here_doc_expander(&s, env);
			ft_putendl_fd(s, fd);
			free(s);
			s = readline("> ");
		}
		if (end_by_delimiter == false)
		{
			ft_putendl_fd("bash: warning: here-document delimited by end-of-file (wanted '", STDERR_FILENO);
			ft_putstr_fd(redirect->filename, STDERR_FILENO);
			ft_putendl_fd("`)", STDERR_FILENO);
		}
		exit(0);
	}
	setup_parent_wait_signal_handlers();
	waitpid(pid, &status, 0);
	setup_interactive_signal_handlers();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
		ft_putendl_fd("", STDERR_FILENO);
        close(fd);
		env->prev_exit_status = -1;
        return (NULL);
    }
	close(fd);
	return (tmpfile);
}
