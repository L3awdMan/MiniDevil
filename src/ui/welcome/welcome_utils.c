/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:50:10 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:06:57 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/**
 * @brief Sleep for approximately ms millisecondes using a little trick
 * 
 * - Sets VMIN = 0 so read() doesn't wait for a character
 * - Sets VTIME = ms / 100 so read() blocks for that many deciseconds before
 * returning with nothing
 * - The result is thus an imitation of a sleep that doesn't use usleep() which
 * is not allowed by the Minishell subject
 * - Original terminal settings are saved and restored after the read
 * 
 * @param ms Milliseconds to sleep
 * @warning Because of VTIME resolution, precision is limited to ~100 ms
 */
void	ft_msleep(int ms)
{
	struct termios	old;
	struct termios	tmp;
	char			buf;

	if (ms <= 0)
		return ;
	tcgetattr(STDIN_FILENO, &old);
	tmp = old;
	tmp.c_cc[VMIN] = 0;
	tmp.c_cc[VTIME] = ms / 100;
	tcsetattr(STDIN_FILENO, TCSANOW, &tmp);
	read(STDIN_FILENO, &buf, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

/**
 * @brief Get the terminal size and calculate centre coordinates
 * 
 * Falls back to 80x24 if ioctl fails
 * 
 * @param t Welcome terminal structure to fill
 */
void	get_welcome_term_size(t_welcome_term *t)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		t->width = 80;
		t->height = 24;
	}
	else
	{
		t->width = ws.ws_col;
		t->height = ws.ws_row;
	}
	t->center_x = t->width / 2;
	t->center_y = t->height / 2;
}

/**
 * @brief Move the terminal cursor to [row, col] and print str
 * 
 * @param row Target row (starting at 1)
 * @param col Target column (starting at 1)
 * @param str String to print at the given position
 */
void	print_at_pos(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

/**
 * @brief Print string centered on the given row
 * 
 * @param t Terminal dimensions
 * @param row Row to print on
 * @param str String to print (ANSI supported)
 * @param len Visible character count of str (used for centering)
 */
void	print_centered(t_welcome_term *t, int row, const char *str, int len)
{
	int	col;

	col = (t->width - len) / 2;
	if (col < 1)
		col = 1;
	print_at_pos(row, col, str);
}
