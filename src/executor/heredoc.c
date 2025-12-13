/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:58:03 by baelgadi          #+#    #+#             */
/*   Updated: 2025/12/13 06:37:53 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the received delimiter is quoted
 * 
 * If the delimiter was quoted ('EOF' or "EOF") then no expansion occurs
 * @param delimiter The delimiter string
 * @return 1 if quoted and 0 if not
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
 * @brief Remove quotes from delimiter if they exist
 * 
 * Transforming 'EOF' or "EOF" to EOF
 * @param delimiter The original delimiter
 * @return A newly allocated unquoted delimiter
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
		expanded = expand_variables(line, env, QUOTE_NONE);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
}

/**
 * @brief Read heredoc input until delimiter
 * 
 * @param fd The write end of the pipe
 * @param delimiter The cleaned delimiter
 * @param env The environment list for variable expansion
 * @param expand Flag to know whether we expand variables or not
 * (0 of delimiter was quoted)
 * @return 0 on success and 1 on CTRL C interrupt
 */
static int	read_heredoc_lines(int fd, char *delimiter, t_env *env, int expand)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (1);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
			break ;
		}
		if (ft_strncmp(line, delimiter, -1) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line, env, expand);
		free(line);
	}
	return (0);
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
	{
		close(pipe_fd[0]);
		restore_stdin();
		return (-1);
	}
	return (pipe_fd[0]);
}
