/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waifu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:29 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:27:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

void		draw_happy(int y, int x);
void		draw_upset(int y, int x);
void		draw_proud(int y, int x);

void	update_waifu_mood(t_ui *ui, int mood)
{
	ui->waifu.mood = mood;
}

static void	render_waifu_mood(t_ui *ui, int y, int x)
{
	if (ui->waifu.mood == MOOD_UPSET)
		draw_upset(y, x);
	else if (ui->waifu.mood == MOOD_PROUD)
		draw_proud(y, x);
	else
		draw_happy(y, x);
}

void	draw_waifu_box(t_ui *ui)
{
	t_box	b;

	b.y = 2;
	b.x = ui->term.width - WAIFU_BOX_W;
	b.h = 9;
	b.w = WAIFU_BOX_W;
	b.color = C_FIRE4;
	draw_box_title(&b, "Neko-chan");
	draw_box_sides(&b);
	draw_box_bottom(&b);
	render_waifu_mood(ui, b.y, b.x);
}
