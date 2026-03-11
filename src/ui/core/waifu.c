/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waifu.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:29 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 06:40:16 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Set the waifu mood
 * 
 * @param ui UI state
 * @param mood New mood value
 */
void	update_waifu_mood(t_ui *ui, int mood)
{
	ui->waifu.mood = mood;
}

/**
 * @brief Dispatch to correct drawing function for the current mood
 * 
 * @param ui UI state
 * @param y Top row
 * @param x Left col
 */
static void	render_waifu_mood(t_ui *ui, int y, int x)
{
	if (ui->waifu.mood == MOOD_UPSET)
		draw_upset(y, x);
	else if (ui->waifu.mood == MOOD_PROUD)
		draw_proud(y, x);
	else
		draw_happy(y, x);
}

/**
 * @brief Draw the waifu panel in the right sidebar
 * 
 * @param ui UI state
 */
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
