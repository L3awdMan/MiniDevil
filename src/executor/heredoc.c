/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:58:03 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/18 19:25:31 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the received delimiter is quoted
 *
 * Determines whether heredoc content should be expanded.
 * Per POSIX: if delimiter has ANY quotes, content is NOT expanded.
 *
 * @param delimiter The raw delimiter string from tokenizer
 * @return 1 if quoted (content NOT expanded), 0 if unquoted (content expanded)
 *
 * @note Examples:
 *       - `<< EOF`    -> unquoted, content IS expanded
 *       - `<< $VAR`   -> unquoted, content IS expanded
 *       - `<< "EOF"`  -> quoted, content NOT expanded
 *       - `<< '$VAR'` -> quoted, content NOT expanded
 */
static int	is_delimiter_quoted(char *delimiter)
{
	if (!delimiter || !*delimiter)
		return (0);
	if (delimiter[0] == '\'' || delimiter[0] == '"')
		return (1);
	return (0);
}

/**
 * @brief Remove quotes from delimiter without variable expansion
 *
 * Per POSIX, the heredoc delimiter is NEVER expanded, regardless of
 * quote type. This function only removes surrounding quotes.
 *
 * @param delimiter The raw delimiter string from tokenizer
 * @return Newly allocated delimiter with quotes removed, NULL on error
 *
 * @note Examples:
 *       - `$VAR`   -> `$VAR` (unchanged, literal)
 *       - `"$VAR"` -> `$VAR` (quotes removed, NOT expanded)
 *       - `'$VAR'` -> `$VAR` (quotes removed, NOT expanded)
 *       - `EOF`    -> `EOF` (unchanged)
 */
static char	*clean_delimiter(char *delimiter)
{
	int		len;
	char	quote;

	if (!delimiter || !*delimiter)
		return (ft_strdup(""));
	if (delimiter[0] != '\'' && delimiter[0] != '"')
		return (ft_strdup(delimiter));
	quote = delimiter[0];
	len = ft_strlen(delimiter);
	if (len >= 2 && delimiter[len - 1] == quote)
		return (ft_substr(delimiter, 1, len - 2));
	return (ft_strdup(delimiter));
}

/**
 * @brief Write one line to the heredoc pipe
 *
 * Expand variables if the delimiter was not quoted
 * @param fd The write end of the pipe
 * @param line The line to write
 * @param env The environment list for variable expansion
 * @param expand Flag to know whether we expand variables or not
 * (0 of delimiter was quoted)
 */
static void	write_heredoc_line(int fd, char *line, t_env *env, int expand)
{
	char	*expanded;

	if (expand && line)
	{
		expanded = expand_variables(line, env, QUOTE_NONE, 0);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
}

/**
 * @brief Read heredoc input until delimiter
 */
static int	read_heredoc_lines(int fd, char *delimiter, t_env *env, int expand)
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
			return (ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2), 0);
		if (!ft_strncmp(line, delimiter, -1))
			return (free(line), 0);
		write_heredoc_line(fd, line, env, expand);
		free(line);
	}
}

/**
 * @brief Main handler for heredoc
 *
 * 1. Check quote status of delimiter and decide on expansion or not
 * 2. Clean the delimiter (unquote it)
 * 3. Create a pipe
 * 4. Set up signal handlers (SIGINT must close STDIN)
 * 5. Read input loop
 * 6. Restore signals and close the write end of the pipe
 * @param received_delimiter The raw delimiter received from the parser
 * @param env The environment list for variable expansion
 * @return File descriptor of the read end of pipe or -1 on error
 */
int	handle_heredoc(char *received_delimiter, t_env *env)
{
	int		pipe_fd[2];
	int		expand;
	char	*delimiter;
	int		interrupted;

	expand = !is_delimiter_quoted(received_delimiter);
	delimiter = clean_delimiter(received_delimiter);
	if (!delimiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (free(delimiter), perror("minishell: pipe"), -1);
	setup_heredoc_signals();
	g_signal = 0;
	interrupted = read_heredoc_lines(pipe_fd[1], delimiter, env, expand);
	setup_interactive_signals();
	close(pipe_fd[1]);
	free(delimiter);
	if (interrupted)
		return (close(pipe_fd[0]), restore_stdin(), -1);
	return (pipe_fd[0]);
}
