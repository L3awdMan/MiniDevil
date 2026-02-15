#include "minishell_ui.h"
#include "signals.h"

/* External global from main project's signals.c */
extern volatile sig_atomic_t	g_signal;

/* Static storage for shell pointer needed by signal handler */
static t_shell	*g_ui_shell = NULL;

/*
** Handles SIGWINCH - redraws UI on terminal resize.
** Uses the shell structure to access the UI instead of a global.
*/
static void	handle_ui_sigwinch(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)info;
	(void)context;
	
	/* Use the saved shell pointer */
	if (g_ui_shell && g_ui_shell->ui)
	{
		get_term_size(&g_ui_shell->ui->term);
		draw_ui(g_ui_shell->ui);
	}
}

/*
** Sets up signal handlers for UI mode.
** Uses simple handlers that work with raw terminal mode (no readline).
*/
void	setup_ui_signals(t_shell *shell)
{
	struct sigaction	sa_winch;
	struct sigaction	sa_int;

	/* Save shell pointer for signal handler access */
	g_ui_shell = shell;

	/* Set up SIGWINCH handler for terminal resize */
	ft_memset(&sa_winch, 0, sizeof(sa_winch));
	sa_winch.sa_sigaction = handle_ui_sigwinch;
	sigemptyset(&sa_winch.sa_mask);
	sa_winch.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGWINCH, &sa_winch, NULL);

	/* Ignore SIGINT in UI mode - raw mode handles Ctrl+C differently */
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
}
