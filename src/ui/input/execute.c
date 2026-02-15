#include "minishell_ui.h"

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

static void	exec_child(int *pipe_fd, int *saved_fds, t_shell *shell)
{
	int	status;

	close(pipe_fd[0]);
	close(saved_fds[0]);
	close(saved_fds[1]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(pipe_fd[1], STDERR_FILENO);
	close(pipe_fd[1]);
	status = process_ui_input(shell->ui->cmd.buf, shell);
	exit(status);
}

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
