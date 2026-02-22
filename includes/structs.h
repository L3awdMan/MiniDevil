/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 02:24:43 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/22 01:08:32 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/**
 * @file structs.h
 * @brief All data structures used in the project
 *
 * Centralized structure definitions to avoid circular dependencies
 */

//==================================================
//==================== ENUMS =======================
//==================================================

/**
 * @brief Token types produced by the lexer
 *
 * - **TOKEN_WORD**			Regular word (command, argument, filename)
 * - **TOKEN_PIPE**			Pipe operator (|)
 * - **TOKEN_REDIR_IN**		Input redirection (<)
 * - **TOKEN_REDIR_OUT**	Output redirection (>)
 * - **TOKEN_APPEND**		Append redirection (>>)
 * - **TOKEN_HEREDOC**		Here document (<<)
 */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}					t_token_type;

/**
 * @brief Quote types (for variable expansion control)
 *
 * - Single quotes 'text' prevents all expansion
 * - Double quotes "text" allows $variable expansion
 * - No quotes, normal expansion and word splitting
 *
 * **Example**
 * 	- echo '$HOME'		=> $HOME (literal)
 * 	- echo "$HOME"		=> /home/user (expanded)
 * 	- echo $HOME		=> /home/user (expanded)
 */
typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_type;

/**
 * @brief AST Node types
 *
 * Determines what kind of operation each AST node represents
 *
 * - **NODE_COMMAND**		Simple command (echo hello)
 * - **NODE_PIPE**			Pipeline (cmd1 | cmd2)
 * - **NODE_REDIR_IN**		Input redirect (< file)
 * - **NODE_REDIR_OUT**		Output redirect (> file)
 * - **NODE_REDIR_APPEND**	Append redirect (>> file)
 * - **NODE_REDIR_HEREDOC**	Here document (<< EOF)
 */
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_REDIR_HEREDOC
}					t_node_type;

//==================================================
//================ TOKEN STRUCTURES ================
//==================================================

/**
 * @brief Single token from Lexer output
 *
 * Represents 1 lexical unit from the input
 * Tokens form a linked list that gets passed to the parser
 */

// How this token was quoted (handled in expansion)
// Actual text content ("echo", "|", "file.txt",...)
// Type of token (word, pipe, redirection, ...)
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	int				connected;
	struct s_token	*next;
}					t_token;

//==================================================
//============= ENVIRONMENT STRUCTURE ==============
//==================================================

/**
 * @brief Environment variable node
 *
 * @details Why doubly linked list?
 * - Easy insertion/deletion for export and unset
 * - Can traverse forward & backward
 * - Each node holds ONE environment variable
 *
 * List visualization:
 *
 * [HOME=/home/user] <-> [PATH=/bin] <-> [USER=john] <-> NULL
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

//==================================================
//================= AST STRUCTURES =================
//==================================================

/**
 * @brief Command node data
 *
 * Stores data for NODE_COMMAND type
 * Actually holds the current command and its arguments
 *
 * Example for *echo an example*
 * 	- args = ["echo", "an", "example", NULL]
 *  - argc = 3
 */
typedef struct s_cmd_node
{
	char			**args;
	int				argc;
}					t_cmd_node;

/**
 * @brief Redirection node data
 *
 * Stores data for Redirection type nodes
 * Links a file to a command for I/O redirection
 *
 * Example for *cat > output.txt*
 * 	- file = "output.txt"
 * 	- cmd = [node for "cat"]
 * 	- redir_type = NODE_REDIR_OUT
 * 	- heredoc_fd = pipe read-end from handle_heredoc()
 */
typedef struct s_redir_node
{
	char			*file;
	struct s_ast	*cmd;
	t_node_type		redir_type;
	int				quote;
	int				heredoc_fd;
}					t_redir_node;

/**
 * @brief Binary operation node data (for pipes)
 *
 * Stores data for NODE_PIPE type
 * COnnects two commands in a pipeline
 *
 * Example for *echo example | cat*
 * 	- Left = [node for "echo example"]
 *  - Right = [node for "cat"]
 */
typedef struct s_binary_node
{
	struct s_ast	*left;
	struct s_ast	*right;
}					t_binary_node;

/**
 * @brief Union for the AST node
 *
 * This union stores the data for exactly one AST node type
 * Members are:
 * - cmd (data for a command node)
 * - redir (data for a redirection node)
 * - binary (data for a binary node = pipe)
 * @see t_ast
 */
typedef union u_ast_data
{
	t_cmd_node		cmd;
	t_redir_node	redir;
	t_binary_node	binary;
}					t_ast_data;

/**
 * @brief AST node - main tree structure
 *
 * Uses a UNION to save memory (only stores one type at a time)
 * A node is ALWAYS only one thing: COMMAND, PIPE or REDIRECTION
 *
 * Memory savings with union:
 * 	- Without union: 56 bytes (all 3 structs)
 * 	- With union: 24 bytes (size of largest = t_redir_node)
 * 	- Saving 32 bytes per node!
 *
 * @warning ALWAYS check 'type' BEFORE accessing union members
 */
typedef struct s_ast
{
	t_node_type		type;
	t_ast_data		data;
}					t_ast;

//==================================================
//================ SHELL STRUCTURE =================
//==================================================

/**
 * @brief Main shell state container
 *
 * Stores all shell state in one place for easy parameter parsing
 *
 * @var current_ast   Tracks the AST root during execution so forked
 *                    child processes (pipes) can free it before exit()
 * @var current_input Tracks the current input line so forked child
 *                    processes (pipes) can free it before exit()
 */
typedef struct s_shell
{
	t_env			*env;
	int				exit_status;
	int				running;
	int				interactive;
	int				ui_mode;
	struct s_ui		*ui;
	struct s_ast	*current_ast;
	char			*current_input;
}					t_shell;

//==================================================
//================ SYNTAX ERROR CODES ==============
//==================================================

/**
 * @brief Enum for specific syntax error types
 *
 * Allows precise error tracking and better testing
 *
 * - **EMPTY_INPUT**		Empty input string
 * - **PIPE_START**			Pipe at start: | cat
 * - **PIPE_END**			Pipe at end: cat |
 * - **PIPE_DOUBLE**		Double pipe: cat || grep
 * - **PIPE_NO_CMD**		No command after pipe: cat | > out
 * - **REDIR_NO_FILE**		Missing filename: cat <
 * - **REDIR_AFTER_PIPE**	Redirection right after pipe
 */
typedef enum e_syntax_error
{
	ERR_NONE = 0,
	ERR_EMPTY_INPUT,
	ERR_PIPE_START,
	ERR_PIPE_END,
	ERR_PIPE_DOUBLE,
	ERR_PIPE_NO_CMD,
	ERR_REDIR_NO_FILE,
	ERR_REDIR_AFTER_PIPE
}					t_syntax_error;

#endif
