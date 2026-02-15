#include "minishell_ui.h"

/*
** Reads a single key from stdin, handling escape sequences for arrows.
*/
int	read_key(void)
{
	char	c;
	char	seq[3];

	if (read(STDIN_FILENO, &c, 1) != 1)
		return (-1);
	if (c == KEY_ESC)
	{
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return (KEY_ESC);
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return (KEY_ESC);
		if (seq[0] == '[')
		{
			if (seq[1] == 'A')
				return (KEY_UP);
			else if (seq[1] == 'B')
				return (KEY_DOWN);
			else if (seq[1] == 'C')
				return (KEY_RIGHT);
			else if (seq[1] == 'D')
				return (KEY_LEFT);
		}
		return (KEY_ESC);
	}
	return (c);
}

/*
** Adds a character to the command buffer.
*/
void	cmd_add_char(t_cmd *cmd, char c)
{
	if (cmd->len < MAX_CMD_LEN - 1)
	{
		cmd->buf[cmd->len] = c;
		cmd->len++;
		cmd->buf[cmd->len] = '\0';
	}
}

/*
** Deletes the last character from the command buffer.
*/
void	cmd_del_char(t_cmd *cmd)
{
	if (cmd->len > 0)
	{
		cmd->len--;
		cmd->buf[cmd->len] = '\0';
	}
}

/*
** Handles up/down arrow keys for output scrolling.
*/
static void	handle_scroll(t_ui *ui, int key)
{
	if (key == KEY_UP && ui->out.scroll > 0)
	{
		ui->out.scroll--;
		redraw_output_only(ui);
	}
	else if (key == KEY_DOWN && ui->out.scroll < ui->out.count - 10)
	{
		ui->out.scroll++;
		redraw_output_only(ui);
	}
}

/*
** Main key handler - dispatches to appropriate action.
*/
void	handle_key(t_shell *shell, int key)
{
	t_ui	*ui;
	
	if (!shell || !shell->ui)
		return ;
	ui = shell->ui;
	if (key == KEY_ENTER)
		cmd_execute(shell);
	else if (key == KEY_BACKSPACE)
		cmd_del_char(&ui->cmd);
	else if (key == KEY_CTRL_D)
		ui->running = 0;
	else if (key == KEY_CTRL_C)
	{
		ft_memset(ui->cmd.buf, 0, MAX_CMD_LEN);
		ui->cmd.len = 0;
	}
	else if (key == KEY_UP || key == KEY_DOWN)
		handle_scroll(ui, key);
	else if (key >= 32 && key <= 126)
		cmd_add_char(&ui->cmd, (char)key);
}
