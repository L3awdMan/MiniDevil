/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:16 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:37:19 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Handle "exit" and "clear" commands directly
 * 
 * - `exit` sets ui->running = 0
 * - `clear` calls out_clear()
 * 
 * @param shell Shell state
 * @return 1 if the command was handled as UI builtin and 0 otherwise
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

/**
 * @brief Execute the current command buffer & reset it
 * 
 * - Return silently if shell/ui is NULL or if the buffer was empty
 * - Try exec_ui_builtin() first, then try execute_minishell_cmd()
 * 
 * @param shell Shell state
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
