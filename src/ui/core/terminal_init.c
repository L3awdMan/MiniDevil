/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:21 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 02:08:56 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Intializes the command buffer, output state and waifu mood
 * 
 * - Allocate the lines pointer array (MAX_LINES)
 * - Sets running to 0 and prints to STDERR on allocation failure
 * - Default waifu mood is MOOD_PROUD
 * 
 * @param ui UI state
 */
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

/**
 * @brief Prepare the terminal and UI state for the main event
 * 
 * - Get terminal size, enable raw mode, hide cursor and clean screen
 * - Call init_ui_state() to inialize the rest of the UI struct to 0
 * 
 * @param ui 
 */
void	init_term(t_ui *ui)
{
	get_term_size(&ui->term);
	set_raw_mode(&ui->term);
	write(STDOUT_FILENO, CURSOR_HIDE, ft_strlen(CURSOR_HIDE));
	write(STDOUT_FILENO, CLEAR_SCREEN, ft_strlen(CLEAR_SCREEN));
	init_ui_state(ui);
}

/**
 * @brief Clean up the UI terminal state and free output memory
 * 
 * - Clear the screen, restore cursor and restore original terminal mode
 * - Free output lines array via out_free
 * 
 * @param ui 
 */
void	cleanup_term(t_ui *ui)
{
	write(STDOUT_FILENO, CLEAR_SCREEN, ft_strlen(CLEAR_SCREEN));
	write(STDOUT_FILENO, CURSOR_SHOW, ft_strlen(CURSOR_SHOW));
	restore_term_mode(&ui->term);
	out_free(&ui->out);
}
