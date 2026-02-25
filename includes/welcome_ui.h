/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome_ui.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by Shawz             #+#    #+#             */
/*   Updated: 2026/02/25 04:52:36 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WELCOME_UI_H
# define WELCOME_UI_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <unistd.h>

/* Include main UI header for shared definitions */
# ifndef MINISHELL_UI_H
#  include "minishell_ui.h"
# endif

//──────────────────────────────────────────────────
//───────── WELCOME SCREEN COLOR PALETTE ───────────
//        (Fire & Blood theme - Hell mode)          
//──────────────────────────────────────────────────

/* Additional cursor controls for welcome screen */
# define HIDE_CURSOR "\033[?25l"
# define SHOW_CURSOR "\033[?25h"

/* Blood gradient (Dark Red -> Bright Red - borders, demon body, loading bg) */
# define C_AURA1 "\033[38;5;52m"
# define C_AURA2 "\033[38;5;88m"
# define C_AURA3 "\033[38;5;124m"
# define C_AURA4 "\033[38;5;160m"
# define C_AURA5 "\033[38;5;196m"

/* Fire gradient (Red -> Orange -> Yellow - title, prompt, cursor, eyes) */
# define C_M1 "\033[38;5;196m"
# define C_M2 "\033[38;5;202m"
# define C_M3 "\033[38;5;208m"
# define C_M4 "\033[38;5;214m"
# define C_M5 "\033[38;5;220m"

//──────────────────────────────────────────────────
//────────────────── STRUCTURE ─────────────────────
//──────────────────────────────────────────────────

/**
 * @brief Extended terminal info (for welcome screen pos)
 */
typedef struct s_welcome_term
{
	int		width;	/**< Terminal width */
	int		height;	/**< Terminal height */
	int		center_x;	/**< Horizontal center col */
	int		center_y;	/**< Vertical center row */
}			t_welcome_term;

//──────────────────────────────────────────────────
//───────────── DEMON ART DIMENSIONS ───────────────
//──────────────────────────────────────────────────

# define DEMON_WIDTH 60
# define DEMON_HEIGHT 16
# define DEMON_LINES 16

//──────────────────────────────────────────────────
//────────────────── FUNCTIONS ─────────────────────
//──────────────────────────────────────────────────

//────────────── welcome.c ────────────────

void		draw_welcome(void);

//────────────── welcome_effects.c ────────────────

void		animate_title(t_welcome_term *t);

void		animate_quote(t_welcome_term *t);

void		animate_credits(t_welcome_term *t);

void		animate_loading(t_welcome_term *t);

//────────────── welcome_demon.c ────────────────

void		animate_demon(t_welcome_term *t);

void		flash_demon_eyes(t_welcome_term *t);

const char	*get_demon_art_line(int line);

//────────────── welcome_utils.c ────────────────

void		get_welcome_term_size(t_welcome_term *t);

void		print_at_pos(int row, int col, const char *str);

void		print_centered(t_welcome_term *t, int row, const char *str,
				int len);

#endif
