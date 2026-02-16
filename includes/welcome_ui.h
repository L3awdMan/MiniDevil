/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_ui.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Shawz <shawz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by Shawz             #+#    #+#             */
/*   Updated: 2025/11/17 00:00:00 by Shawz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WELCOME_UI_H
# define WELCOME_UI_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <stdio.h>

/* Include main UI header for shared definitions */
# ifndef MINISHELL_UI_H
#  include "minishell_ui.h"
# endif

/*
** ══════════════════════════════════════════════════════════════════════════
**                    WELCOME SCREEN COLOR PALETTE
**                     (Blood/Fire theme - Hell mode)
** ══════════════════════════════════════════════════════════════════════════
*/

/* Additional cursor controls for welcome screen */
# define HIDE_CURSOR    "\033[?25l"
# define SHOW_CURSOR    "\033[?25h"

/* Blood gradient (Dark Red -> Bright Red - borders, demon body, loading bg) */
# define C_AURA1        "\033[38;5;52m"
# define C_AURA2        "\033[38;5;88m"
# define C_AURA3        "\033[38;5;124m"
# define C_AURA4        "\033[38;5;160m"
# define C_AURA5        "\033[38;5;196m"

/* Fire gradient (Red -> Orange -> Yellow - title, prompt, cursor, eyes) */
# define C_MYSTIC1      "\033[38;5;196m"
# define C_MYSTIC2      "\033[38;5;202m"
# define C_MYSTIC3      "\033[38;5;208m"
# define C_MYSTIC4      "\033[38;5;214m"
# define C_MYSTIC5      "\033[38;5;220m"

/*
** ══════════════════════════════════════════════════════════════════════════
**                          WELCOME-SPECIFIC STRUCTURES
** ══════════════════════════════════════════════════════════════════════════
*/

/* Extended terminal structure for welcome screen */
typedef struct s_welcome_term
{
	int	width;
	int	height;
	int	center_x;
	int	center_y;
}	t_welcome_term;

/*
** ══════════════════════════════════════════════════════════════════════════
**                           DEMON ART DIMENSIONS
** ══════════════════════════════════════════════════════════════════════════
*/

# define DEMON_WIDTH    60
# define DEMON_HEIGHT   16
# define DEMON_LINES    16

/*
** ══════════════════════════════════════════════════════════════════════════
**                          FUNCTION PROTOTYPES
** ══════════════════════════════════════════════════════════════════════════
*/

/* welcome.c - Main welcome screen */
void	draw_welcome(void);

/* welcome_effects.c - Visual effects */
void	animate_title(t_welcome_term *t);
void	animate_quote(t_welcome_term *t);
void	animate_credits(t_welcome_term *t);
void	animate_loading(t_welcome_term *t);

/* welcome_demon.c - Demon art and effects */
void	animate_demon(t_welcome_term *t);
void	flash_demon_eyes(t_welcome_term *t);
const char	*get_demon_art_line(int line);

/* welcome_utils.c - Utility functions */
void	get_welcome_term_size(t_welcome_term *t);
void	print_at_pos(int row, int col, const char *str);
void	print_centered(t_welcome_term *t, int row, const char *str, int len);
int		ft_strlen_visible(const char *str);

#endif
