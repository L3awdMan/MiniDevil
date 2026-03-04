/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_ui.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by L3awd             #+#    #+#             */
/*   Updated: 2026/03/04 04:28:55 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_UI_H
# define MINISHELL_UI_H

# include <sys/ioctl.h>
# include <termios.h>
# include "structs.h"
# include "libft.h"
# include "ft_printf.h"
# include "signals.h"

// ──────────────────────────────────────────────────
// ────────────── ANSI ESCAPE CODES ─────────────────
// ──────────────────────────────────────────────────

/* Cursor & Screen Control */
# define ESC_SEQ "\033["
# define CLEAR_SCREEN "\033[2J\033[H"
# define CLEAR_LINE "\033[2K"
# define CURSOR_HIDE "\033[?25l"
# define CURSOR_SHOW "\033[?25h"
# define SAVE_CURSOR "\033[s"
# define RESTORE_CURSOR "\033[u"

/* Text Styles */
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define DIM "\033[2m"

/* Colors - 256-color mode for WSL compatibility */
# define CYAN "\033[38;5;51m"
# define PURPLE "\033[38;5;141m"
# define GREEN "\033[38;5;47m"
# define RED "\033[38;5;196m"
# define YELLOW "\033[38;5;226m"
# define WHITE "\033[38;5;231m"
# define GRAY "\033[38;5;240m"

/* Fire Gradient Colors (Red → Orange → Yellow) */
# define C_FIRE1 "\033[38;5;196m"
# define C_FIRE2 "\033[38;5;202m"
# define C_FIRE3 "\033[38;5;208m"
# define C_FIRE4 "\033[38;5;214m"
# define C_FIRE5 "\033[38;5;220m"

/* Blood Gradient Colors (Dark Red → Bright Red) */
# define C_BLOOD1 "\033[38;5;52m"
# define C_BLOOD2 "\033[38;5;88m"
# define C_BLOOD3 "\033[38;5;124m"
# define C_BLOOD4 "\033[38;5;160m"
# define C_BLOOD5 "\033[38;5;196m"

// ──────────────────────────────────────────────────
// ────────────── BOX DRAWING CHARS ─────────────────
// ──────────────────────────────────────────────────

# define BOX_TL "╭"
# define BOX_TR "╮"
# define BOX_BL "╰"
# define BOX_BR "╯"
# define BOX_H "─"
# define BOX_V "│"

// ──────────────────────────────────────────────────
// ────────────────── CONSTANTS ─────────────────────
// ──────────────────────────────────────────────────

# define CMD_BOX_HEIGHT 3
# define WAIFU_BOX_W 24
# define EXIT_BOX_H 3
# define MIN_WIDTH 80
# define MIN_HEIGHT 24
# define MAX_CMD_LEN 1024
# define MAX_LINES 1000
# define BUF_SIZE 4096

/* Minishell binary path (relative - expects binary in same directory) */
# define MINISHELL_PATH "./minishell"

/* Key codes */
# define KEY_ESC 27
# define KEY_ENTER 10
# define KEY_BACKSPACE 127
# define KEY_CTRL_D 4
# define KEY_CTRL_C 3
# define KEY_UP 1001
# define KEY_DOWN 1002
# define KEY_LEFT 1003
# define KEY_RIGHT 1004

/* Waifu moods - HAPPY (default), PROUD (success), UPSET (failure) */
# define MOOD_HAPPY 0
# define MOOD_UPSET 1
# define MOOD_PROUD 2

// ──────────────────────────────────────────────────
// ────────────────── STRUCTURES ────────────────────
// ──────────────────────────────────────────────────

/**
 * @brief Box position & style for drawing
 */
typedef struct s_box
{
	int				y;	/**< Top row */
	int				x;	/**< Left col */
	int				h;	/**< Height in rows */
	int				w;	/**< Width in cols */
	const char		*color;	/**< ANSI color str */
}					t_box;

/**
 * @brief Terminal dimensions & saved state
 */
typedef struct s_term
{
	int				width;	/**< Cols */
	int				height;	/**< Rows */
	struct termios	orig;	/**< Original termios settings */
}					t_term;

/**
 * @brief Cmd input buffer
 */
typedef struct s_cmd
{
	char			buf[MAX_CMD_LEN];	/**< Fixed size buffer */
	int				len;	/**< Current input length */
	int				cursor;	/**< Cursor position */
}					t_cmd;

/**
 * @brief Output display state
 */
typedef struct s_out
{
	char			**lines;	/**< Array of output lines */
	int				count;	/**< Nbr of stored lines */
	int				scroll;	/**< Current scroll's offset */
	int				exit_code;	/**< Last command exit code */
}					t_out;

/**
 * @brief Waifu state
 */
typedef struct s_waifu
{
	int				mood;	/**< Current mood */
	int				blink;	/**< !wip Blink animation state */
}					t_waifu;

/**
 * @brief UI state
 */
typedef struct s_ui
{
	t_term			term;	/**< Terminal state */
	t_cmd			cmd;	/**< Cmd buffer */
	t_out			out;	/**< Output state */
	t_waifu			waifu;	/**< Waifu state */
	int				running;	/**< Event loop flag */
}					t_ui;

// ──────────────────────────────────────────────────
// ────────────────── FUNCTIONS ─────────────────────
// ──────────────────────────────────────────────────

// ────────────── ui_main.c ────────────────

void				run_ui_mode(t_shell *shell);

// ────────────── terminal_init.c ────────────────

void				init_term(t_ui *ui);

void				cleanup_term(t_ui *ui);

// ────────────── terminal.c ────────────────

void				get_term_size(t_term *term);

void				set_raw_mode(t_term *term);

void				restore_term_mode(t_term *term);

void				print_goodbye(void);

// ────────────── ui_signals.c ────────────────

void				setup_ui_signals(void);

// ────────────── output.c ────────────────

void				out_add_line(t_out *out, const char *line);

void				out_clear(t_out *out);

void				out_free(t_out *out);

void				add_welcome_msg(t_ui *ui);

// ────────────── waifu.c ────────────────

void				update_waifu_mood(t_ui *ui, int mood);

void				draw_waifu_box(t_ui *ui);

// ────────────── drawing_utils.c ────────────────

void				print_at(int row, int col, const char *str);

void				draw_hline(int width, const char *color);

// ────────────── drawing_text.c ────────────────

char				*truncate_line(const char *line, int max_w);

int					visual_strlen(const char *s);

// ────────────── drawing_box.c ────────────────

void				draw_box(t_box *b);

void				draw_box_title(t_box *b, const char *title);

void				draw_box_sides(t_box *b);

void				draw_box_bottom(t_box *b);

// ────────────── drawing.c ────────────────

void				draw_ui(t_ui *ui);

void				draw_cmd_box(t_ui *ui);

void				draw_exit_box(t_ui *ui);

void				redraw_cmd_only(t_ui *ui);

// ────────────── drawing_output.c ────────────────

void				draw_out_line(t_ui *ui, int y, int i, int w);

void				draw_out_box(t_ui *ui);

void				redraw_output_only(t_ui *ui);

// ────────────── input.c ────────────────

int					read_key(void);

void				cmd_add_char(t_cmd *cmd, char c);

void				cmd_del_char(t_cmd *cmd);

void				handle_key(t_shell *shell, int key);

// ────────────── command.c ────────────────

void				cmd_execute(t_shell *shell);

// ────────────── execute.c ────────────────

void				execute_minishell_cmd(t_shell *shell);

// ────────────── execute_utils.c ────────────────

void				read_output_from_fd(t_ui *ui, int fd);

int					process_ui_input(char *input, t_shell *shell);

// ────────────── welcome_utils.c ────────────────

void				ft_msleep(int ms);

// ────────────── welcome.c ────────────────

void				draw_welcome(void);

#endif
