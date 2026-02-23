/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:29:04 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

void	draw_cmd_box(t_ui *ui)
{
	t_box	b;
	int		clear_len;

	b.y = 2;
	b.x = 2;
	b.h = 3;
	b.w = ui->term.width - WAIFU_BOX_W - 4;
	b.color = C_FIRE3;
	draw_box_title(&b, "Cmdline");
	draw_box_sides(&b);
	draw_box_bottom(&b);
	print_at(b.y + 1, b.x + 1, C_FIRE4 BOLD " ❯ " RESET);
	ft_printf("%s", ui->cmd.buf);
	write(STDOUT_FILENO, "█", 3);
	clear_len = b.w - ui->cmd.len - 6;
	while (clear_len-- > 0)
		write(STDOUT_FILENO, " ", 1);
}

void	draw_exit_box(t_ui *ui)
{
	t_box	b;
	char	*color;

	b.y = 11;
	b.x = ui->term.width - WAIFU_BOX_W;
	b.h = EXIT_BOX_H;
	b.w = WAIFU_BOX_W;
	b.color = C_FIRE2;
	draw_box(&b);
	if (ui->out.exit_code == 0)
		color = C_FIRE5;
	else
		color = C_FIRE1;
	print_at(b.y + 1, b.x + 4, color);
	ft_printf("%s Exit: %d %s", BOLD, ui->out.exit_code, RESET);
}

void	draw_ui(t_ui *ui)
{
	write(STDOUT_FILENO, CLEAR_SCREEN, ft_strlen(CLEAR_SCREEN));
	draw_cmd_box(ui);
	draw_waifu_box(ui);
	draw_exit_box(ui);
	draw_out_box(ui);
}

void	redraw_cmd_only(t_ui *ui)
{
	int	width;
	int	clear_len;

	width = ui->term.width - WAIFU_BOX_W - 4;
	print_at(3, 4, C_FIRE4 BOLD "❯" RESET " ");
	write(STDOUT_FILENO, ui->cmd.buf, ui->cmd.len);
	write(STDOUT_FILENO, "█", 3);
	clear_len = width - ui->cmd.len - 8;
	if (clear_len > 0)
	{
		while (clear_len-- > 0)
			write(STDOUT_FILENO, " ", 1);
	}
}
