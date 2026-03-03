/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:11:06 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/02 06:20:13 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"
#include "minishell.h"

static void	handle_ui_sigwinch(int sig)
{
	g_signal = sig;
}

void	setup_ui_signals(void)
{
	struct sigaction	sa_winch;
	struct sigaction	sa_int;

	ft_memset(&sa_winch, 0, sizeof(sa_winch));
	sa_winch.sa_handler = handle_ui_sigwinch;
	sigemptyset(&sa_winch.sa_mask);
	sa_winch.sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &sa_winch, NULL);
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
}
