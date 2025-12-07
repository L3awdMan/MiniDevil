/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 18:19:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/07 21:32:52 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"

// Node types - what kind of operation is this?
//
// Simple command: echo hello
// Pipeline: cmd1 | cmd2
// Input: < file
// Output: > file
// Append: >> file
// Heredoc: << EOF

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_REDIR_HEREDOC
}					t_node_type;

//===================== STRUCTURE: t_cmd_node ============================
//
// PURPOSE:
//    Store data for a COMMAND node
//    Holds the actual command and its arguments
//
// FIELDS:
//    char **args - Array of strings: ["echo", "hello", NULL]
//    int argc    - Count of arguments (2 in example above)
//
// WHY WE NEED IT:
//    When node type is NODE_COMMAND, we need to store:
//    - The command name ("echo")
//    - All its arguments ("hello")
//    - How many args we have
//
// MEMORY SIZE:
//    sizeof(char *) + sizeof(int) = usually 16 bytes
//
// EXAMPLE:
//    For command "echo hello world":
//    cmd_node.args = ["echo", "hello", "world", NULL]
//    cmd_node.argc = 3

typedef struct s_cmd_node
{
	char			**args;
	int				argc;
}					t_cmd_node;

//===================== STRUCTURE: t_redir_node ==========================
//
// PURPOSE:
//    Store data for a REDIRECTION node
//    Holds filename and the command being redirected
//
// FIELDS:
//    char *file           - Filename: "output.txt"
//    struct s_ast *cmd    - The command: e.g., cmd("cat")
//    t_node_type redir_type - Which redirect: <, >, <<, >>
//
// WHY WE NEED IT:
//    When node type is NODE_REDIR_*, we need to store:
//    - WHAT file (filename)
//    - WHICH command (the cmd being redirected)
//    - HOW to redirect (in/out/append/heredoc)
//
// MEMORY SIZE:
//    sizeof(char *) + sizeof(void *) + sizeof(int) = usually 24 bytes
//
// EXAMPLE:
//    For "cat > output.txt":
//    redir_node.file = "output.txt"
//    redir_node.cmd = [cmd_node for "cat"]
//    redir_node.redir_type = NODE_REDIR_OUT
//
//========================================================================

typedef struct s_redir_node
{
	char			*file;
	struct s_ast	*cmd;
	t_node_type		redir_type;
}					t_redir_node;

//===================== STRUCTURE: t_binary_node =========================
//
// PURPOSE:
//    Store data for a BINARY operation node (2 sides)
//    Holds left and right children (for PIPE)
//
// FIELDS:
//    struct s_ast *left  - Left side command
//    struct s_ast *right - Right side command
//
// WHY WE NEED IT:
//    When node type is NODE_PIPE, we need to store:
//    - LEFT command (before the |)
//    - RIGHT command (after the |)
//
// MEMORY SIZE:
//    sizeof(void *) + sizeof(void *) = usually 16 bytes
//
// EXAMPLE:
//    For "echo hello | cat":
//    binary_node.left = [cmd_node for "echo hello"]
//    binary_node.right = [cmd_node for "cat"]

typedef struct s_binary_node
{
	struct s_ast	*left;
	struct s_ast	*right;
}					t_binary_node;

//===================== STRUCTURE: t_ast =================================
//
// PURPOSE:
//    Main AST node - can represent ANY type of node
//    Uses UNION to save memory (only stores ONE type at a time)
//
// FIELDS:
//    t_node_type type - WHICH type is this node?
//    union data {...}  - The ACTUAL data (only ONE of the three!)
//
// THE UNION TRICK:
//    Instead of allocating space for ALL THREE:
//       [X]  t_cmd_node cmd;     (16 bytes)
//       [X]  t_redir_node redir; (24 bytes)
//       [X]  t_binary_node binary; (16 bytes)
//       TOTAL: 56 bytes per node!
//
//    Union allocates space for ONLY THE BIGGEST:
//       ONE of: cmd OR redir OR binary
//       TOTAL: 24 bytes (size of biggest = t_redir_node)
//       SAVINGS: 32 bytes per node!
//
// HOW IT WORKS:
//    1. Check the 'type' field
//    2. Based on type, use the correct union member:
//       if (type == NODE_COMMAND)     → use data.cmd
//       if (type == NODE_PIPE)        → use data.binary
//       if (type == NODE_REDIR_OUT)   → use data.redir
//
// MEMORY LAYOUT:
//    [type: 4 bytes][union: 24 bytes] = 28 bytes total
//
//    The SAME 24 bytes can hold:
//    - cmd data (when type = NODE_COMMAND)
//    - redir data (when type = NODE_REDIR_*)
//    - binary data (when type = NODE_PIPE)
//
// WHY UNION HERE:
//    A node is ALWAYS only ONE thing:
//    - Either a COMMAND
//    - OR a PIPE
//    - OR a REDIRECTION
//    NEVER two at the same time!
//    So we don't need space for all three!
//
// EXAMPLE USAGE:
//    t_ast *node = malloc(sizeof(t_ast));
//
//    // Creating a COMMAND node:
//    node->type = NODE_COMMAND;
//    node->data.cmd.args = ["echo", "hi", NULL];
//    // DON'T use data.binary or data.redir - they're INVALID!
//
//    // Creating a PIPE node:
//    node->type = NODE_PIPE;
//    node->data.binary.left = left_cmd;
//    node->data.binary.right = right_cmd;
//    // DON'T use data.cmd or data.redir - they're INVALID!
//
// CRITICAL RULE:
//    ALWAYS check 'type' BEFORE accessing union members!
//    if (node->type == NODE_COMMAND)
//        printf("%s", node->data.cmd.args[0]);  // SAFE
//    else
//        // DON'T touch data.cmd - UNDEFINED BEHAVIOR!

typedef struct s_ast
{
	t_node_type type; // "What am I?" (COMMAND? PIPE? REDIR?)
	union
	{
		t_cmd_node cmd;       // If I'm a COMMAND
		t_redir_node redir;   // If I'm a REDIRECTION
		t_binary_node binary; // If I'm a PIPE
	} data;                   // "My actual data" (only ONE is valid at a time!)
}					t_ast;

// AST node creation functions
t_ast				*create_cmd_node(char **args, int argc);
t_ast				*create_pipe_node(t_ast *left, t_ast *right);
t_ast				*create_redir_node(t_node_type type, char *file,
						t_ast *cmd);

// AST recursive tree cleanup
void				free_ast(t_ast *node);

#endif
