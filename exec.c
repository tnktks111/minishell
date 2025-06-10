/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:15:54 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/10 22:05:42 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
exec_ast();
exec_and_or();
long exec_pipeline(t_tree_node *root)
{
    t_tree_node *curr;
    t_tree_node **cmd_list;
    int i;
    int cmd_cnt;
    
    cmd_cnt = 1;
    curr = root->left;
    while (curr->kind == NODE_PIPE)
    {
        cmd_cnt++;
        curr = curr->left;
    }
    cmd_list = (t_tree_node **)malloc(sizeof(t_tree_node *) * cmd_cnt);
    i = -1;
    while (++i < cmd_cnt)
    {
        cmd_list[i] = curr;
        if (i != 0)
            curr = curr->parent;
        curr = curr->parent->right;
    }
    
}

long exec_cmd_lst(t_tree_node **lst, int size)
{
    int i = -1;
    int pid;
    long status;

    while(++i < size)
    {
        pid = fork();
        if (pid == -1)
            break;
        if (pid == 0)
            exec_command_helper();
    }
    waitpid(pid, &status, 0);
    i = -1;
    while(++i < size - 1)
        wait(NULL);
    return (status);
}