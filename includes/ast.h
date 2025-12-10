/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:19:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 02:15:43 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/**
 * @file ast.h
 * @brief Abstract Syntax Tree creation & management
 *
 * The AST is the parsed structure of a command
 * It's a tree where:
 * 	- Leaf nodes are commands
 * 	- Internal nodes are operators (pipe / redirections)
 *
 * For more details @see !wip
 */

# include "minishell.h"
# include "structs.h"

//==================================================
//================ NODE CREATION ===================
//==================================================

/**
 * @brief Create a command node
 *
 * @param args NULL terminated argument array
 * @param argc Arguments count
 * @return New command node or NULL on failure
 */
t_ast	*create_cmd_node(char **args, int argc);

/**
 * @brief Create a pipe node connecting 2 commands
 *
 * @param left Left side (stdout goes to pipe)
 * @param right Right side (stdin comes from pipe)
 * @return New pipe node or NULL on failure
 */
t_ast	*create_pipe_node(t_ast *left, t_ast *right);

/**
 * @brief Create a redirection node
 *
 * @param type Redirection type (IN, OUT, APPEND, HEREDOC)
 * @param file Target filename
 * @param cmd Command to redirect (can be NULL if chained redirection)
 * @return New redirection node or NULL on failure
 */
t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd);

//==================================================
//=================== CLEANUP ======================
//==================================================

/**
 * @brief Recursively free an AST
 *
 * Walk the tree and free all nodes and their data
 * SAfe to call with NULL too
 *
 * @param node Root of the tree to free
 */
void	free_ast(t_ast *node);

#endif
