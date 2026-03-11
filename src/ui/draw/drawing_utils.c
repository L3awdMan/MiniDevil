/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:09 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 03:34:02 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Move the terminal cursor to [row, col] and write str
 * 
 * @param row Target row (starting at 1)
 * @param col Target colum (starting at 1)
 * @param str String to print
 */
void	print_at(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

/**
 * @brief Draw a horizontal line of dashes with optional color
 * 
 * @param width Number of dash characters to write
 * @param color ANSI color escape string to apply
 */
void	draw_hline(int width, const char *color)
{
	int	i;

	if (color)
		write(STDOUT_FILENO, color, ft_strlen(color));
	i = 0;
	while (i++ < width)
		write(STDOUT_FILENO, "─", 3);
	if (color)
		write(STDOUT_FILENO, RESET, ft_strlen(RESET));
}
