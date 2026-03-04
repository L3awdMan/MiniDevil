/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:51:45 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "token.h"
#include "libft.h"

/**
 * @brief Expand variables in a single arg string
 * 
 * @param arg Raw argument string
 * @param shell Shell context (for env and exit status)
 * @return Newly allocated expanded string or NULL if failed
 */
static char	*expand_arg(char *arg, t_shell *shell)
{
	return (expand_variables(arg, shell->env, QUOTE_NONE, shell->exit_status));
}

/**
 * @brief Expand variables in all aguments of a command
 * 
 * Allocates a new array where elements are expanded
 * 
 * @param args Argument array (NULL terminated)
 * @param shell Shell context (for env and exit status)
 * @return Newly allocated expanded string or NULL if failed
 */
char	**expand_args(char **args, t_shell *shell)
{
	char	**expanded;
	int		i;

	if (!args)
		return (NULL);
	expanded = malloc(sizeof(char *) * (ft_arrlen(args) + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded[i] = expand_arg(args[i], shell);
		if (!expanded[i])
		{
			ft_free_strarray(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

/**
 * @brief Execute a NODE_COMMAND AST node
 * 
 * @param node AST node of type NODE_COMMAND
 * @param shell Shell context
 * @return Exit status of the executed command
 */
static int	exec_command_node(t_ast *node, t_shell *shell)
{
	char	**args;

	args = node->data.cmd.args;
	return (exec_simple_command(args, shell));
}

/**
 * @brief Main AST executor that dispatches by node type
 * 
 * - Recursively walks the AST and dispatches NODE_COMMAND, NODE_PIPE and
 * NODE_REDIR to exec_command_node(), handle_pipe() and handle_redir()
 * 
 * @param node Root of the AST subtree to execute
 * @param shell Shell context
 * @return Exit status of the executed pipe/command and 0 for NULL nodes
 */
int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (exec_command_node(node, shell));
	if (node->type == NODE_PIPE)
		return (handle_pipe(node, shell));
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
		return (handle_redir(node, shell));
	return (0);
}
