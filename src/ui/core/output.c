/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zotaj-di <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:13:48 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/17 17:13:48 by zotaj-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/* Add line to output */
void	out_add_line(t_out *out, const char *line)
{
	if (out->count < MAX_LINES)
	{
		out->lines[out->count] = ft_strdup(line);
		out->count++;
	}
}

/* Clear output */
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

/* Free output */
void	out_free(t_out *out)
{
	out_clear(out);
	free(out->lines);
}

/* Adds the welcome message box to the output panel */
void	add_welcome_msg(t_ui *ui)
{
	out_add_line(&ui->out, "");
	out_add_line(&ui->out, "  ╔══════════════════════════════════════════╗");
	out_add_line(&ui->out, "  ║  Welcome to Minishell UI!                ║");
	out_add_line(&ui->out, "  ║                                          ║");
	out_add_line(&ui->out, "  ║  • Type commands and press Enter         ║");
	out_add_line(&ui->out, "  ║  • Use ↑/↓ to scroll output              ║");
	out_add_line(&ui->out, "  ║  • Press Ctrl-D to exit                  ║");
	out_add_line(&ui->out, "  ║  • Type 'clear' to clear output          ║");
	out_add_line(&ui->out, "  ║                                          ║");
	out_add_line(&ui->out, "  ╚══════════════════════════════════════════╝");
	out_add_line(&ui->out, "");
}
