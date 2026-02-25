/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 03:16:21 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/25 05:10:15 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "structs.h"

//────────────── executor.c ────────────────

int		executor(t_ast *node, t_shell *shell);

//────────────── executor_redir.c ────────────────

int		open_redir_file(
			char *file, t_node_type type, int quoted, t_shell *shell);

int		setup_redirection(int fd, t_node_type type);

void	restore_fd(int saved_fd, t_node_type type);

int		handle_redir(t_ast *node, t_shell *shell);

//────────────── executor_pipe.c ────────────────

void	exec_right_pipe_child(t_ast *right, int pipe_fd[2], t_shell *shell);

void	exec_left_pipe_child(t_ast *left, int pipe_fd[2], t_shell *shell);

int		handle_pipe(t_ast *node, t_shell *shell);

//────────────── exec_utils.c ────────────────

int		exec_cmd_not_found(char *cmd);

int		is_builtin(char *cmd);

int		exec_builtin(char **args, t_shell *shell);

int		exec_simple_command(char **args, t_shell *shell);

int		pipe_fork_error(int pipe_fd[2], pid_t left_pid);

//────────────── path.c ────────────────

char	*find_cmd_path(char *cmd, t_env *env);

//────────────── exec_cmd.c ────────────────

int		exec_external(char **args, t_env *env);

//────────────── heredoc.c ────────────────

int		handle_heredoc(char *delimiter, int quoted, t_shell *shell);

//────────────── heredoc_collect.c ────────────────

int		walk_heredocs(t_ast *node, t_shell *shell);

int		collect_heredocs(t_ast *node, t_shell *shell);

#endif
