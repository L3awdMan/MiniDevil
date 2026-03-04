/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:16 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 02:12:25 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Get the terinal dimensions via ioctl and store them
 * 
 * Fallback is 80x24 (default) in case ioctl fails
 * 
 * @param term Terminal state to fill with width and height
 */
void	get_term_size(t_term *term)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
	{
		term->width = ws.ws_col;
		term->height = ws.ws_row;
	}
	else
	{
		term->width = 80;
		term->height = 24;
	}
}

/**
 * @brief Switch the terminal into non canonical and no echo mode
 * 
 * - Save original termios settings in term->orig for later
 * - Disable ECHO and ICANON & set VMIN = 1 and VTIME = 0
 * 
 * @param term Terminal state (original is saved before modification)
 */
void	set_raw_mode(t_term *term)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &term->orig);
	raw = term->orig;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/**
 * @brief Restore the terminal to the saved settings
 * 
 * @param term Terminal state (holds the saved orig termios)
 */
void	restore_term_mode(t_term *term)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term->orig);
}

/**
 * @brief Print the exit banner to STDOUT
 */
void	print_goodbye(void)
{
	ft_printf("\n" PURPLE BOLD);
	ft_printf("╔════════════════════════════════════════╗\n");
	ft_printf("║                                        ║\n");
	ft_printf("║           See you next time !          ║\n");
	ft_printf("║                                        ║\n");
	ft_printf("╚════════════════════════════════════════╝\n" RESET "\n");
}
