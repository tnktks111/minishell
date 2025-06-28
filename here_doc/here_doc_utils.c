/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:30:34 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/28 22:26:31 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		file_iswdir(char *filepath);
/*check node_simplecmd and delete tmpfile if the node has REDIR_HEREDOCs*/
void	unlink_tmpfile(t_tree_node *node_simplecmd);
/*check all simplecmd nodes below the node_pipeline 
and apply unlink_tmpfile to them*/
void	unlink_all_tmpfiles(t_tree_node *node_pipeline);

void	unlink_tmpfile(t_tree_node *node_simplecmd)
{
	t_redirect	*curr;

	curr = node_simplecmd->data.command.redirects;
	while (curr)
	{
		if (curr->kind == REDIR_HEREDOC)
			unlink(curr->filename);
		curr = curr->next;
	}
}

void	unlink_all_tmpfiles(t_tree_node *node_pipeline)
{
	t_tree_node	*curr;

	curr = node_pipeline->left;
	while (curr->kind == NODE_PIPE)
		curr = curr->left;
	unlink_tmpfile(curr);
	curr = curr->parent;
	while (curr->kind != NODE_PIPE_LINE)
	{
		unlink_tmpfile(curr->right);
		curr = curr->parent;
	}
}

int	file_iswdir(char *filepath)
{
	struct stat	sb;

	if (stat(filepath, &sb))
		return (0);
	return (S_ISDIR(sb.st_mode) && access(filepath, W_OK) == 0);
}
