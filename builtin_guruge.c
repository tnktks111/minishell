
#include "minishell.h"

// unsigned char	builtin_cd(t_tree_node *node, t_env *env)
// {
// 	char	*path;

// 	if (node->data.command.args[2])
// 		return (1); // INVALid SYNTAX
// 	else if (!node->data.command.args[1])
// 		return (chdir($HOME)); // MOVE HOME
// 	node->data.command.args[1] = path;
// 	if (ft_strncmp(path, "-", 2) == 0)
// 		return (chdir($OLDPWD));
// }
