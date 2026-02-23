/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:19:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:36:31 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"
# include "structs.h"

t_ast	*create_cmd_node(char **args, int argc);

t_ast	*create_pipe_node(t_ast *left, t_ast *right);

t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd, int quote);

void	free_ast(t_ast *node);

#endif
