/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_demon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:04:37 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:56:35 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/**
 * @brief Return 1 line of the ASCII Demon art by index
 * 
 * @param line Line index (0 to DEMON_LINES - 1)
 * @return Pointer to the art string or NULL if out of range
 */
const char	*get_demon_art_line(int line)
{
	static const char	*demon_art[] = {
		"                            ,-.",
		"       ___,---.__          /'|`\\          __,---,___",
		"    ,-'    \\`    `-.____,-'  |  `-.____,-'    //    `-.",
		"  ,'        |           ~'\\     /`~           |        `.",
		" /      ___//              `. ,'          ,  , \\___      \\",
		"|    ,-'   `-.__   _         |        ,    __,-'   `-.    |",
		"|   /          /\\_  `   .    |    ,      _/\\          \\   |",
		"\\  |           \\ \\`-.___ \\   |   / ___,-'/ /           |  /",
		" \\  \\           | `._   `\\\\  |  //'   _,' |           /  /",
		"  `-.\\         /'  _ `---'' , . ``---' _  `\\         /,-'",
		"     ``       /     \\    ,='/ \\`=.    /     \\       ''",
		"             |__   /|\\_,--.,-.--,--._/|\\   __|",
		"             /  `./  \\\\`\\ |  |  | /,//' \\,'  \\",
		"            /   /     ||--+--|--+-/-|     \\   \\",
		"           |   |     /'\\_\\_\\ | /_/_/`\\     |   |",
		"             `-._,-'   `-._______,-'   `-._,-'"
	};

	if (line < 0 || line >= DEMON_LINES)
		return (NULL);
	return (demon_art[line]);
}

/**
 * @brief Map a Demon art row to its gradient color string
 * 
 * C_AURA 5 = brightest and C_AURA 1 = darkest
 * 
 * @param line Art line index
 * @return ANSI color escape string for that row
 */
static const char	*get_demon_color(int line)
{
	if (line < 3)
		return (C_AURA5);
	if (line < 6)
		return (C_AURA4);
	if (line < 9)
		return (C_AURA3);
	if (line < 12)
		return (C_AURA2);
	return (C_AURA1);
}

/**
 * @brief Print 1 demon art line at the correct terminal position, with color
 * 
 * Centers the line by using DEMON_WIDTH
 * 
 * @param t Terminal dimensions (for centering)
 * @param line Art line index
 * @param row Terminal row to print on
 */
static void	print_demon_line(t_welcome_term *t, int line, int row)
{
	int			col;
	const char	*color;
	const char	*art_line;

	art_line = get_demon_art_line(line);
	if (!art_line)
		return ;
	color = get_demon_color(line);
	col = (t->width - DEMON_WIDTH) / 2;
	if (col < 1)
		col = 1;
	print_at_pos(row, col, color);
	ft_printf("%s%s", art_line, RESET);
}

/**
 * @brief Animate the Demon art with 100ms delay
 * 
 * @param t Terminal dimensions for centering and row calculations
 */
void	animate_demon(t_welcome_term *t)
{
	int	start_row;
	int	i;

	start_row = t->center_y - 4;
	i = -1;
	while (++i < DEMON_LINES)
	{
		print_demon_line(t, i, start_row + i);
		ft_msleep(100);
	}
}

/**
 * @brief Animate the demon's eyes blinking
 * 
 * @param t Terminal dimensions to calculate the eye positions
 */
void	flash_demon_eyes(t_welcome_term *t)
{
	int	row;
	int	col;

	row = t->center_y - 4 + 8;
	col = (t->width - DEMON_WIDTH) / 2 + 22;
	print_at_pos(row, col, C_M1 BOLD "_" RESET);
	print_at_pos(row, col + 13, C_M1 BOLD "_" RESET);
	ft_msleep(100);
	print_at_pos(row, col, C_M5 BOLD "*" RESET);
	print_at_pos(row, col + 13, C_M5 BOLD "*" RESET);
	ft_msleep(150);
	print_at_pos(row, col, C_M1 BOLD "_" RESET);
	print_at_pos(row, col + 13, C_M1 BOLD "_" RESET);
}
