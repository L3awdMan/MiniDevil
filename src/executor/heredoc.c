/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:58:03 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/23 22:20:34 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(int fd, char *line, t_shell *shell, int expand)
{
	char	*expanded;

	if (expand && line)
	{
		expanded = expand_variables(line, shell->env, QUOTE_NONE,
				shell->exit_status);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
}

static int	read_heredoc_lines(int fd, char *delim, t_shell *shell, int expand)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
			if (line && ft_strlen(line) && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (g_signal == SIGINT)
			return (1);
		if (!line)
			return (ft_putstr_fd("minishell: warning: heredoc delim by EOF\n",
					2), 0);
		if (!ft_strncmp(line, delim, -1))
			return (free(line), 0);
		write_heredoc_line(fd, line, shell, expand);
		free(line);
	}
}

int	handle_heredoc(char *delimiter, int quoted, t_shell *shell)
{
	int		pipe_fd[2];
	int		expand;
	int		interrupted;

	expand = !quoted;
	if (pipe(pipe_fd) == -1)
		return (perror("minishell: pipe"), -1);
	setup_heredoc_signals();
	g_signal = 0;
	interrupted = read_heredoc_lines(pipe_fd[1], delimiter, shell, expand);
	setup_interactive_signals();
	close(pipe_fd[1]);
	if (interrupted)
		return (close(pipe_fd[0]), restore_stdin(), -1);
	return (pipe_fd[0]);
}
