/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:58:03 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/03 06:35:13 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Write one heredoc line (optionally expanding variables)
 * 
 * @param fd Write end of the heredoc pipe
 * @param line Line to write
 * @param shell Shell context (for variable expansion)
 * @param expand Whether to expand variables or not
 */
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

/**
 * @brief Read a line from STDIN
 * 
 * Reads lines interactively (readline) or from a pipe (get_next_line)
 * 
 * @return The line
 */
static char	*read_heredoc_line(void)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
		return (readline("> "));
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

/**
 * @brief Read the lines until the delimiter or EOF/SIGINT
 * 
 * Each line is written to fd via write_heredoc_line()
 * 
 * @param fd Write end of the heredoc pipe
 * @param delim Delimiter for the heredoc
 * @param shell Shell context (for variable expansion)
 * @param expand Whether to expand variables or not
 * @return 0 on normal (delimiter/EOF) and 1 on SIGINT
 */
static int	read_heredoc_lines(int fd, char *delim, t_shell *shell, int expand)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (g_signal == SIGINT)
		{
			free(line);
			return (1);
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delim by EOF\n", 2);
			return (0);
		}
		if (!ft_strncmp(line, delim, -1))
		{
			free(line);
			return (0);
		}
		write_heredoc_line(fd, line, shell, expand);
		free(line);
	}
}

/**
 * @brief Execute a heredoc (read input and return a readable fd)
 * 
 * - Creates a pipe
 * - Reads lines into the write end until the delimiter is reached
 * - Closes the write end and returns the read end
 * - If the delimiter was quoted, no expansion occurs
 * - On SIGINT closes both pipe ends and restores STDIN
 * 
 * @param delimiter Heredoc delimiter
 * @param quoted Whether the delimiter was quoted or not
 * @param shell Shell context (for signals and variable expansion)
 * @return Read end fd of the heredoc pipe or -1 on error/interrupted
 */
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
	{
		close(pipe_fd[0]);
		restore_stdin();
		return (-1);
	}
	return (pipe_fd[0]);
}
