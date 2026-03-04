/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:11:06 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:56:09 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_ui.h"

/**
 * @brief Records SIGWINCH in the global signal flag
 * 
 * @param sig Signal number received (SIGWINCH)
 */
static void	handle_ui_sigwinch(int sig)
{
	g_signal = sig;
}

/**
 * @brief Set up SIGWINCH and SIGINT handlers for the UI loop
 * 
 * - SIGWINCH is handled by handle_ui_sigwinch() with SA_RESTART
 * - SIGINT is ignored so CTRL C doesn't kill the UI
 */
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
