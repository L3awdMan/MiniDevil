/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:11:44 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 07:59:28 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

/**
 * @brief Convert a token type to the corresponding AST redirection node type
 * 
 * @param type Token type
 * @return NODE_REDIR_... type or NODE_COMMAND (fallback)
 */
static t_node_type	get_redir_node_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (NODE_REDIR_APPEND);
	if (type == TOKEN_HEREDOC)
		return (NODE_REDIR_HEREDOC);
	return (NODE_COMMAND);
}

/**
 * @brief Parse a single redirection token and the following WORD (filename)
 * 
 * @param tokens Pointer to current token pointer (advanced)
 * @param cmd Command to wrap
 * @return Redirection node or NULL on error
 * @warning cmd is freed on failure (do not use after NULL return)
 */
static t_ast	*parse_one_redirection(t_token **tokens, t_ast *cmd)
{
	t_node_type	type;
	char		*file;
	int			quoted;

	type = get_redir_node_type((*tokens)->type);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		free_ast(cmd);
		return (NULL);
	}
	file = ft_strdup((*tokens)->value);
	if (!file)
	{
		free_ast(cmd);
		return (NULL);
	}
	quoted = ((*tokens)->quote_type != QUOTE_NONE);
	*tokens = (*tokens)->next;
	return (create_redir_node(type, file, cmd, quoted));
}

/**
 * @brief Collect trailing WORD tokens and merge them into a command's args
 * 
 * Handles cases such as "cmd arg1 > out arg2" as it collects new_args,
 * allocates a merged array and copies old + new args, then updates
 * cmd->data.cmd.args and argc
 * 
 * @param cmd Command node to extend
 * @param tokens Pointer to current token pointer
 */
static void	collect_and_merge_remaining_args(t_ast *cmd, t_token **tokens)
{
	char	**new_args;
	char	**merged;
	int		new_argc;
	int		i;

	new_args = collect_args(tokens, &new_argc);
	if (!new_args)
		return ;
	merged = malloc(sizeof(char *) * (cmd->data.cmd.argc + new_argc + 1));
	if (!merged)
	{
		ft_free_strarray(new_args);
		return ;
	}
	i = -1;
	while (++i < cmd->data.cmd.argc)
		merged[i] = cmd->data.cmd.args[i];
	i = -1;
	while (++i < new_argc)
		merged[cmd->data.cmd.argc + i] = new_args[i];
	merged[cmd->data.cmd.argc + new_argc] = NULL;
	free(cmd->data.cmd.args);
	free(new_args);
	cmd->data.cmd.args = merged;
	cmd->data.cmd.argc += new_argc;
}

/**
 * @brief Extract the innermost command from a redirection chain
 * 
 * @param node Starting node of the chain
 * @return Innermost non redirection node or NULL
 */
static t_ast	*get_command_node(t_ast *node)
{
	while (node && node->type >= NODE_REDIR_IN
		&& node->type <= NODE_REDIR_HEREDOC)
		node = node->data.redir.cmd;
	return (node);
}

/**
 * @brief Parse a command with all its redirections
 * 
 * Parses a simple command and wraps it in redirection nodes as long as
 * redirection tokens follow. After each redirection it collects any trailing
 * word tokens and adds them to the command's args
 * 
 * @param tokens Pointer to current token pointer (advanced)
 * @return Command AST or NULL on error
 * @note It reverses the redirection chain for left to right execution
 */
t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;
	t_ast	*cmd_node;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = parse_simple_command(tokens);
	while (*tokens && is_redirection((*tokens)->type))
	{
		cmd = parse_one_redirection(tokens, cmd);
		if (!cmd)
			return (NULL);
		cmd_node = get_command_node(cmd);
		if (cmd_node)
			collect_and_merge_remaining_args(cmd_node, tokens);
	}
	return (reverse_redir_chain(cmd));
}
