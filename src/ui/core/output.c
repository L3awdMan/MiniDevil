/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:13:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 02:09:02 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Append a duplicate of a line to the output buffer
 * 
 * Do nothing if the buffers already reached MAX_LINES
 * 
 * @param out Output to append to
 * @param line String to store
 */
void	out_add_line(t_out *out, const char *line)
{
	if (out->count < MAX_LINES)
	{
		out->lines[out->count] = ft_strdup(line);
		out->count++;
	}
}

/**
 * @brief Free stored output lines and reset the scroll state
 * 
 * @param out Output state to clear
 */
void	out_clear(t_out *out)
{
	int	i;

	i = 0;
	while (i < out->count)
	{
		free(out->lines[i]);
		out->lines[i] = NULL;
		i++;
	}
	out->count = 0;
	out->scroll = 0;
}

/**
 * @brief Clear all output lines and free the lines pointer array
 * 
 * @param out Output to release
 */
void	out_free(t_out *out)
{
	out_clear(out);
	free(out->lines);
}

/**
 * @brief Fill the output buffer w/ an introductory welcome banner
 * 
 * @param ui UI state for which output buffer receives the banner
 */
void	add_welcome_msg(t_ui *ui)
{
	out_add_line(&ui->out, "");
	out_add_line(&ui->out, "  ╔══════════════════════════════════════════╗");
	out_add_line(&ui->out, "  ║                                          ║");
	out_add_line(&ui->out, "  ║  Welcome to Minishell UI!                ║");
	out_add_line(&ui->out, "  ║                                          ║");
	out_add_line(&ui->out, "  ║  • Type commands and press Enter         ║");
	out_add_line(&ui->out, "  ║  • Press Ctrl-D to exit                  ║");
	out_add_line(&ui->out, "  ║  • Type 'clear' to clear output          ║");
	out_add_line(&ui->out, "  ║                                          ║");
	out_add_line(&ui->out, "  ╚══════════════════════════════════════════╝");
	out_add_line(&ui->out, "");
}
