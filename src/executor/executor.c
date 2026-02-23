/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:20:45 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_arg(char *arg, t_shell *shell)
{
	return (expand_variables(arg, shell->env, QUOTE_NONE, shell->exit_status));
}

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

static int	exec_command_node(t_ast *node, t_shell *shell)
{
	char	**args;

	args = node->data.cmd.args;
	return (exec_simple_command(args, shell));
}

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
