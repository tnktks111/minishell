/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttanaka <ttanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:22:41 by ttanaka           #+#    #+#             */
/*   Updated: 2025/06/15 20:22:03 by ttanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char perror_string(char *str)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    perror(str);
    return (EXIT_FAILURE);
}

void error_cmd_not_found(char *cmd_name)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

void error_too_many_args(char *cmd_name)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

void error_numeric_arg_required(char *cmd_name, char *arg)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": numeric argument required\n");
}

void error_not_a_valid_identifier(char *cmd_name, char *arg)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": numeric argument required\n");
}

void error_illegal_arguments(char *cmd_name, char *arg)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd(": you must not pass any arguments to the command\n");
}

void error_invalid_option(char *cmd_name, char *arg)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd_name, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putchar_fd(arg[0], STDERR_FILENO);
    ft_putchar_fd(arg[1], STDERR_FILENO);
    ft_putstr_fd(": invalid option\n");
}