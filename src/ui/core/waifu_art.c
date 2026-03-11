/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waifu_art.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:48:34 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 02:25:52 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Draw happy Neko chan ASCII art
 * 
 * @param y Top row of the first line
 * @param x Left col of the first art line
 */
void	draw_happy(int y, int x)
{
	print_at(y + 1, x + 2, "  ✨ />  フ     ");
	print_at(y + 2, x + 2, "     |  ^  ^|    ");
	print_at(y + 3, x + 2, "   /`ミ_ω_彡  ");
	print_at(y + 4, x + 2, "   /     |       ");
	print_at(y + 5, x + 2, "  /  ヽ   ﾉ    ");
	print_at(y + 6, x + 2, " / ￣|   | | ♡ ");
	print_at(y + 7, x + 2, " | ( ￣ヽ__ヽ_)");
}

/**
 * @brief Draw upset Neko chan ASCII art
 * 
 * @param y Top row of the first line
 * @param x Left col of the first art line
 */
void	draw_upset(int y, int x)
{
	print_at(y + 1, x + 2, "     />  フ     ");
	print_at(y + 2, x + 2, "     | ˃  ˂|   ");
	print_at(y + 3, x + 2, "   /`ミ_Д_彡  ");
	print_at(y + 4, x + 2, "   /     | 💢  ");
	print_at(y + 5, x + 2, "  /  ヽ   ﾉ   ");
	print_at(y + 6, x + 2, " / ￣| | |hmph");
	print_at(y + 7, x + 2, " ~~( ￣ヽ__ヽ_)");
}

/**
 * @brief Draw proud Neko chan ASCII art
 * 
 * @param y Top row of the first line
 * @param x Left col of the first art line
 */
void	draw_proud(int y, int x)
{
	print_at(y + 1, x + 2, "   ✨/>  フ ✨ ");
	print_at(y + 2, x + 2, "     | ◕ ‿◕|   ");
	print_at(y + 3, x + 2, "   /`ミ ω 彡  ");
	print_at(y + 4, x + 2, "   /     | ★   ");
	print_at(y + 5, x + 2, "  /  ヽ   ﾉ   ");
	print_at(y + 6, x + 2, " / ￣| | NICE!");
	print_at(y + 7, x + 2, " | (^￣ヽ__ヽ_)");
}
