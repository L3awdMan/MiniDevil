/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:23 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:58:07 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"
#include "token.h"
#include "parser.h"
#include "ast.h"
#include "executor.h"

/**
 * @brief PRocess 1 output character from the child pipe into line buffers
 * 
 * - On `\n` it flushes the current line to out_add_line() & resets len
 * - Otherwise it appends c to line if below BUF_SIZE - 2
 * 
 * @param ui UI state
 * @param c Character read from the pipe
 * @param line Working buffer (accumulates the current line)
 * @param line_len Pointer to the current position in the line
 */
static void	process_output_char(t_ui *ui, char c, char *line, int *line_len)
{
	if (c == '\n')
	{
		line[*line_len] = '\0';
		out_add_line(&ui->out, line);
		*line_len = 0;
	}
	else if (*line_len < BUF_SIZE - 2)
	{
		line[*line_len] = c;
		(*line_len)++;
	}
}

/**
 * @brief Read all available bytes from fd and split into output lines
 * 
 * Reads in chunks of BUF_SIZE and any unterminated line is flushed after EOF
 * 
 * @param ui UI states
 * @param fd File descriptor to read from (pipe read end)
 */
void	read_output_from_fd(t_ui *ui, int fd)
{
	char	buf[BUF_SIZE];
	char	line[BUF_SIZE];
	int		line_len;
	int		bytes;
	int		i;

	line_len = 0;
	bytes = read(fd, buf, sizeof(buf) - 1);
	while (bytes > 0)
	{
		buf[bytes] = '\0';
		i = -1;
		while (++i < bytes)
			process_output_char(ui, buf[i], line, &line_len);
		bytes = read(fd, buf, sizeof(buf) - 1);
	}
	if (line_len > 0)
	{
		line[line_len] = '\0';
		out_add_line(&ui->out, line);
	}
}

/**
 * @brief Full pipeline inside the UI
 * 
 * tokenize -> expand_all_tokens -> parse -> collect_heredocs -> executor
 * 
 * @param input Command string to execute
 * @param shell Shell state (provides environment and execution context)
 * @return Exit status of the command, 1 on expand error, 2 on failure and
 * 130 on heredoc interruption
 */
int	process_ui_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast;
	int		status;

	tokens = tokenize(input);
	if (!tokens)
		return (2);
	if (expand_all_tokens(tokens, shell) < 0)
	{
		free_token_list(tokens);
		return (1);
	}
	ast = parse(tokens);
	free_token_list(tokens);
	if (!ast)
		return (2);
	if (collect_heredocs(ast, shell) == -1)
	{
		free_ast(ast);
		return (130);
	}
	status = executor(ast, shell);
	free_ast(ast);
	return (status);
}
