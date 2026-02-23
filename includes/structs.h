/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 02:24:43 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:39:26 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}					t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}					t_quote_type;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_REDIR_HEREDOC
}					t_node_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_type	quote_type;
	int				connected;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef struct s_cmd_node
{
	char			**args;
	int				argc;
}					t_cmd_node;

typedef struct s_redir_node
{
	char			*file;
	struct s_ast	*cmd;
	t_node_type		redir_type;
	int				quote;
	int				heredoc_fd;
}					t_redir_node;

typedef struct s_binary_node
{
	struct s_ast	*left;
	struct s_ast	*right;
}					t_binary_node;

typedef union u_ast_data
{
	t_cmd_node		cmd;
	t_redir_node	redir;
	t_binary_node	binary;
}					t_ast_data;

typedef struct s_ast
{
	t_node_type		type;
	t_ast_data		data;
}					t_ast;

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
