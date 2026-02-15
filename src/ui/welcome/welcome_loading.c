#include "welcome_ui.h"

/*
** Displays the credits line.
*/
void	animate_credits(t_welcome_term *t)
{
	int			row;
	const char	*credits;

	credits = "✧ Crafted by Lhorse & Pedro ✧";
	row = t->center_y + 9;
	print_centered(t, row, DIM C_AURA4, 0);
	print_at_pos(row, (t->width - 29) / 2, "");
	ft_printf("%s%s%s%s", DIM, C_MYSTIC3, credits, RESET);
	fflush(stdout);
}

/*
** Animates the loading progress bar.
*/
void	animate_loading(t_welcome_term *t)
{
	int			row;
	int			i;

	row = t->center_y + 11;
	print_centered(t, row,
		C_AURA2 "░░░░░░░░░░░░░░░░░░░░" RESET, 20);
	i = -1;
	while (++i < 20)
	{
		print_at_pos(row, (t->width - 20) / 2 + i,
			C_MYSTIC3 "▓" RESET);
		ft_msleep(50);
	}
	ft_msleep(200);
}
