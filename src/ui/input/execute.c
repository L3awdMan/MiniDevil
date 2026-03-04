/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:19 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 05:02:51 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <fcntl.h>
#include "minishell_ui.h"
#include "env.h"
#include "get_next_line.h"

/**
 * @brief Create a pipe and save current STDOUT / STDERR file descriptors
 * 
 * On pipe failure it sets exit_code = 1 and waifu's mood to upset
 * 
 * @param pipe_fd 2 element array to receive the pipe read and write ends
 * @param saved_fds 2 elements array to receive the duped STDOUT and STDERR
 * @param ui UI state
 * @return 0 on success and -1 on pipe failure
 */
static int	init_pipe_and_fds(int *pipe_fd, int *saved_fds, t_ui *ui)
{
	if (pipe(pipe_fd) == -1)
	{
		out_add_line(&ui->out, "Error: pipe creation failed");
		ui->out.exit_code = 1;
		update_waifu_mood(ui, MOOD_UPSET);
		return (-1);
	}
	saved_fds[0] = dup(STDOUT_FILENO);
	saved_fds[1] = dup(STDERR_FILENO);
	return (0);
}

/**
 * @brief Clean up after a failed fork & report the error to the UI
 * 
 * @param ui UI state
 * @param pipe_fd The pipe ends to close
 * @param saved_fds The saved STDOUT / STDERR to close
 */
static void	handle_fork_error(t_ui *ui, int *pipe_fd, int *saved_fds)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(saved_fds[0]);
	close(saved_fds[1]);
	out_add_line(&ui->out, "Error: fork failed");
	ui->out.exit_code = 1;
	update_waifu_mood(ui, MOOD_UPSET);
}

/**
 * @brief Redirect I/O to the pipe and run the command
 * 
 * - Redirects STDIN from `/dev/null` and STDOUT & STDERR to the pipe write end
 * - Calls process_ui_input(), frees memory and exits with the command status
 * 
 * @param pipe_fd Pipe read and write ends
 * @param saved_fds Saved STDOUT / STDERR originals to close
 * @param shell Shell state
 */
static void	exec_child(int *pipe_fd, int *saved_fds, t_shell *shell)
{
	int	status;
	int	dev_null;

	close(pipe_fd[0]);
	close(saved_fds[0]);
	close(saved_fds[1]);
	dev_null = open("/dev/null", O_RDONLY);
	if (dev_null >= 0)
	{
		dup2(dev_null, STDIN_FILENO);
		close(dev_null);
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(pipe_fd[1], STDERR_FILENO);
	close(pipe_fd[1]);
	status = process_ui_input(shell->ui->cmd.buf, shell);
	out_free(&shell->ui->out);
	free(shell->ui);
	free_env_list(&shell->env);
	get_next_line(-42);
	exit(status);
}

/**
 * @brief Collect output, restore I/O and wait for child
 * 
 * - Reads all child output before restoring STDOUT and STDERR
 * - Updates exit_code and waifu mood accordingly
 * 
 * @param pipe_fd The pipe read and write ends
 * @param saved_fds Saved STDOUT / STDERR to restore
 * @param sh Shell state (owns the UI)
 * @param pid PID of the child to wait for
 */
static void	exec_parent(int *pipe_fd, int *saved_fds, t_shell *sh, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	read_output_from_fd(sh->ui, pipe_fd[0]);
	close(pipe_fd[0]);
	dup2(saved_fds[0], STDOUT_FILENO);
	dup2(saved_fds[1], STDERR_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->ui->out.exit_code = WEXITSTATUS(status);
	else
		sh->ui->out.exit_code = 1;
	if (sh->ui->out.exit_code == 0)
		update_waifu_mood(sh->ui, MOOD_PROUD);
	else
		update_waifu_mood(sh->ui, MOOD_UPSET);
}

/**
 * @brief Fork a child to execute a minishell command & capture its output
 * 
 * Creates a pipe, forks, runs exec_child() in the child and exec_parent() in
 * the parent
 * 
 * @param shell Shell state (contains command buffer and UI output)
 */
void	execute_minishell_cmd(t_shell *shell)
{
	int		pipe_fd[2];
	int		saved_fds[2];
	pid_t	pid;

	if (!shell || !shell->ui)
		return ;
	update_waifu_mood(shell->ui, MOOD_HAPPY);
	if (init_pipe_and_fds(pipe_fd, saved_fds, shell->ui) == -1)
		return ;
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error(shell->ui, pipe_fd, saved_fds);
		return ;
	}
	if (pid == 0)
		exec_child(pipe_fd, saved_fds, shell);
	else
		exec_parent(pipe_fd, saved_fds, shell, pid);
}
