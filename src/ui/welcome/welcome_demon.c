/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_demon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:04:37 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/21 00:34:22 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_ui.h"

/*
** Returns a specific line of the demon ASCII art.
** This replaces the global variable with a function.
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

/*
** Returns color for demon line based on row.
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

/*
** Prints a single line of demon art with color.
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

/*
** Animates the demon art appearing line by line.
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

/*
** Animates the demon's eyes flashing.
*/
void	flash_demon_eyes(t_welcome_term *t)
{
	int	row;
	int	col;

	row = t->center_y - 4 + 5;
	col = (t->width - DEMON_WIDTH) / 2 + 26;
	print_at_pos(row, col, C_M1 BOLD "_" RESET);
	print_at_pos(row, col + 8, C_M1 BOLD "_" RESET);
	ft_msleep(100);
	print_at_pos(row, col, C_M5 BOLD "*" RESET);
	print_at_pos(row, col + 8, C_M5 BOLD "*" RESET);
	ft_msleep(150);
	print_at_pos(row, col, C_M1 BOLD "_" RESET);
	print_at_pos(row, col + 8, C_M1 BOLD "_" RESET);
}
