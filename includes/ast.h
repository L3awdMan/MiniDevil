/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:19:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/10 07:35:49 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "structs.h"

// ────────────── ast.c ────────────────

t_ast	*create_cmd_node(char **args, int argc);

t_ast	*create_pipe_node(t_ast *left, t_ast *right);

t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd, int quote);

// ────────────── ast_utils.c ────────────────

void	free_ast(t_ast *node);

// ────────────── B O N U S ────────────────

/** @cond BONUS */

t_ast	*create_and_or_node(t_node_type type, t_ast *left, t_ast *right);

t_ast	*create_subshell_node(t_ast *child);

/** @endcond */

#endif
