/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_output.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:01 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:28:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Write a single scrolled output line's content to STDOUT
 * 
 * - Truncate to w - 4 visible chars if line_idx is within the buffer
 * - Fill remaining width with spaces
 * 
 * @param ui UI state
 * @param line_idx Line index
 * @param w Width of the output box in cols
 */
static void	draw_line_content(t_ui *ui, int line_idx, int w)
{
	char	*line;
	int		clear_len;

	if (line_idx < ui->out.count && ui->out.lines[line_idx])
	{
		line = truncate_line(ui->out.lines[line_idx], w - 4);
		ft_printf("%s", line);
		clear_len = w - 3 - (int)ft_strlen(line);
		if (clear_len > 0)
		{
			while (clear_len-- > 0)
				write(STDOUT_FILENO, " ", 1);
		}
	}
	else
	{
		clear_len = w - 3;
		while (clear_len-- > 0)
			write(STDOUT_FILENO, " ", 1);
	}
}

/**
 * @brief Draw one raw inside the output box (including the side borders)
 * 
 * @param ui UI state
 * @param y Top row of the output box
 * @param i Row index within the box (0 based)
 * @param w Width of the output box
 */
void	draw_out_line(t_ui *ui, int y, int i, int w)
{
	int	line_idx;

	line_idx = ui->out.scroll + i;
	print_at(y + 1 + i, 2, C_FIRE2);
	write(STDOUT_FILENO, BOX_V, 3);
	write(STDOUT_FILENO, RESET, ft_strlen(RESET));
	write(STDOUT_FILENO, " ", 1);
	draw_line_content(ui, line_idx, w);
	print_at(y + 1 + i, 2 + w - 1, C_FIRE2);
	write(STDOUT_FILENO, BOX_V, 3);
	write(STDOUT_FILENO, RESET, ft_strlen(RESET));
}

/**
 * @brief Draw the full output box with title and borders
 * 
 * THe box grows to fill the terminal from row 5 to the last 2 rows
 * 
 * @param ui UI state
 */
void	draw_out_box(t_ui *ui)
{
	t_box	b;
	int		i;

	b.y = 5;
	b.x = 2;
	b.h = ui->term.height - 5 - 2;
	b.w = ui->term.width - WAIFU_BOX_W - 4;
	b.color = C_FIRE2;
	draw_box_title(&b, "Output");
	i = 0;
	while (i < b.h - 2)
	{
		draw_out_line(ui, b.y, i, b.w);
		i++;
	}
	draw_box_bottom(&b);
}

/**
 * @brief Redraw only the output box interior rows without borders nor title
 * 
 * Used after scroll events in order to refresh content without full redraw
 * 
 * @param ui UI state
 */
void	redraw_output_only(t_ui *ui)
{
	int	y;
	int	w;
	int	h;
	int	i;

	y = 5;
	w = ui->term.width - WAIFU_BOX_W - 4;
	h = ui->term.height - 5 - 2;
	i = 0;
	while (i < h - 2)
	{
		draw_out_line(ui, y, i, w);
		i++;
	}
}
