/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/26 03:39:46 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

static void	init_ui_state(t_ui *ui)
{
	ft_memset(ui->cmd.buf, 0, MAX_CMD_LEN);
	ui->cmd.len = 0;
	ui->cmd.cursor = 0;
	ui->out.lines = ft_calloc(MAX_LINES, sizeof(char *));
	if (!ui->out.lines)
	{
		ft_putendl_fd("Error: output buffer allocation failure", STDERR_FILENO);
		ui->running = 0;
		return ;
	}
	ui->out.count = 0;
	ui->out.scroll = 0;
	ui->out.exit_code = 0;
	ui->waifu.mood = MOOD_PROUD;
	ui->running = 1;
}

void	init_term(t_ui *ui)
{
	get_term_size(&ui->term);
	set_raw_mode(&ui->term);
	write(STDOUT_FILENO, CURSOR_HIDE, ft_strlen(CURSOR_HIDE));
	write(STDOUT_FILENO, CLEAR_SCREEN, ft_strlen(CLEAR_SCREEN));
	init_ui_state(ui);
}

void	cleanup_term(t_ui *ui)
{
	write(STDOUT_FILENO, CLEAR_SCREEN, ft_strlen(CLEAR_SCREEN));
	write(STDOUT_FILENO, CURSOR_SHOW, ft_strlen(CURSOR_SHOW));
	restore_term_mode(&ui->term);
	out_free(&ui->out);
}
