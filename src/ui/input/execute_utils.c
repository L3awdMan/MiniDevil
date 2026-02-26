/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:23 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/26 04:15:47 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell_ui.h"
#include "parser.h"
#include "token.h"

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

int	process_ui_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast;
	int		status;

	tokens = tokenize(input);
	if (!tokens)
		return (2);
	if (expand_all_tokens(tokens, shell) < 0)
		return (free_token_list(tokens), 1);
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
