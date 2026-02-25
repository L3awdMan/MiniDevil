/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 02:24:43 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/25 05:10:33 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

//──────────────────────────────────────────────────
//────────────────────── ENUMS ─────────────────────
//──────────────────────────────────────────────────

/**
 * @brief Token types produced by Lexer
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
 * @brief Quote context of a token
 */
typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_type;

/**
 * @brief AST node types
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

//──────────────────────────────────────────────────
//──────────────── TOKEN STRUCTURE ─────────────────
//──────────────────────────────────────────────────

/**
 * @brief Lexer token (singly linked list)
 */
typedef struct s_token
{
	t_token_type	type;	/**< Token type (word, pipe, redir) */
	char			*value;	/**< Text content (allocated) */
	t_quote_type	quote_type;	/**< Quote context */
	int				connected;	/**< 1 if connected */
	struct s_token	*next;	/**< Next token */
}					t_token;

//──────────────────────────────────────────────────
//───────────────── ENV STRUCTURE ──────────────────
//──────────────────────────────────────────────────

/**
 * @brief Environment variable (doubly linked list)
 */
typedef struct s_env
{
	char			*key;	/**< Variable name */
	char			*value;	/**< Value (NULL if export only) */
	struct s_env	*next;	/**< Next node */
	struct s_env	*prev;	/**< Previous node */
}					t_env;

//──────────────────────────────────────────────────
//──────────────── AST STRUCTURES ──────────────────
//──────────────────────────────────────────────────

/**
 * @brief Command data inside AST node
 */
typedef struct s_cmd_node
{
	char			**args;	/**< NULL terminated args array */
	int				argc;	/**< Number of arguments */
}					t_cmd_node;

/**
 * @brief Redirection data inside AST node
 */
typedef struct s_redir_node
{
	char			*file;	/**< Target filename or delimiter */
	struct s_ast	*cmd;	/**< Command subtree */
	t_node_type		redir_type;	/**< Redirection type */
	int				quote;	/**< 1 if delimiter was quoted */
	int				heredoc_fd;	/**< Heredoc pipe fd (-1 if not used)*/
}					t_redir_node;

/**
 * @brief Binary operator data inside AST Node (pipe)
 */
typedef struct s_binary_node
{
	struct s_ast	*left;	/**< left child (before pipe) */
	struct s_ast	*right;	/**< right child (after pipe) */
}					t_binary_node;

/**
 * @brief Only one will be valid depending on node's type
 */
typedef union u_ast_data
{
	t_cmd_node		cmd;
	t_redir_node	redir;
	t_binary_node	binary;
}					t_ast_data;

/**
 * @brief AST node (union based ↑)
 */
typedef struct s_ast
{
	t_node_type		type;	/**< To determine which union member to pick */
	t_ast_data		data;	/**< cmd, redir or binary data */
}					t_ast;

//──────────────────────────────────────────────────
//──────────────── SHELL STRUCTURE ─────────────────
//──────────────────────────────────────────────────

/**
 * @brief Shell state
 */
typedef struct s_shell
{
	t_env			*env;	/**< Environment linked list */
	int				exit_status;	/**< Last command's exit status */
	int				running;	/**< 1 while main loop is active */
	int				interactive;	/**< 1 if STDIN is a tty */
	int				ui_mode;	/**< 1 if UI mode (--ui) */
	struct s_ui		*ui;	/**< UI state (NULL if off) */
	struct s_ast	*current_ast;	/**< Currently executing AST */
	char			*current_input;	/**< Current input line */
}					t_shell;

//──────────────────────────────────────────────────
//────────────── SYNTAX ERROR CODES ────────────────
//──────────────────────────────────────────────────

/**
 * @brief Syntax error codes (for parser)
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
