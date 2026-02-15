#include "minishell_ui.h"

/*
** Prints string at specific row and column position.
*/
void	print_at(int row, int col, const char *str)
{
	ft_printf("\033[%d;%dH%s", row, col, str);
}

/*
** Prints string with specified color and resets.
*/
void	print_color(const char *color, const char *str)
{
	write(STDOUT_FILENO, color, ft_strlen(color));
	write(STDOUT_FILENO, str, ft_strlen(str));
	write(STDOUT_FILENO, RESET, ft_strlen(RESET));
}

/*
** Draws a horizontal line of specified width.
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
