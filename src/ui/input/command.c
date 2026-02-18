/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:16 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/17 18:01:30 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/*
** Executes UI-specific built-in commands (exit, clear).
** Returns 1 if handled.
*/
static int	exec_ui_builtin(t_shell *shell)
{
	t_ui	*ui;

	ui = shell->ui;
	if (ft_strncmp(ui->cmd.buf, "exit", 4) == 0 && (ui->cmd.buf[4] == '\0'
			|| ui->cmd.buf[4] == ' '))
	{
		ui->running = 0;
		return (1);
	}
	if (ft_strncmp(ui->cmd.buf, "clear", 5) == 0 && (ui->cmd.buf[5] == '\0'
			|| ui->cmd.buf[5] == ' '))
	{
		out_clear(&ui->out);
		return (1);
	}
	return (0);
}

/*
** Executes the current command in the buffer.
** Now uses the integrated minishell execution.
*/
void	cmd_execute(t_shell *shell)
{
	t_ui	*ui;

	if (!shell || !shell->ui)
		return ;
	ui = shell->ui;
	if (ui->cmd.len == 0)
		return ;
	out_clear(&ui->out);
	update_waifu_mood(ui, MOOD_HAPPY);
	if (exec_ui_builtin(shell))
	{
		ft_memset(ui->cmd.buf, 0, MAX_CMD_LEN);
		ui->cmd.len = 0;
		update_waifu_mood(ui, MOOD_HAPPY);
		return ;
	}
	execute_minishell_cmd(shell);
	ft_memset(ui->cmd.buf, 0, MAX_CMD_LEN);
	ui->cmd.len = 0;
}
