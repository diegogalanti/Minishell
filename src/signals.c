/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: digallar <digallar@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:11:12 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/02/11 19:36:39 by digallar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal[2];

/* set_signal: This function handles the signals SIGINT (Ctrl-C)
				and SIGQUIT (Ctrl-\).
	SIQUIT is ignored (therefore I just chose the signal function)
	SIGINT should result in displaying a new prompt line. To handle SIGINT
	the function handle_signals() is called with the help of sigaction.
	QUESTION: Is error handling needed?*/

int	set_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	return (0);
}

/* handle_signals: handles SIGINT (more signals could be added)
	a new prompt line is displayed:
	-rl_replace_line empties readline line buffer
		so that the previous user input is not printed
	- a new line is printed
	- the prompt is printed again
	QUESTIONS: How about redirections?
		Should child processes be killed or are they automatically?
		i.e. do child processes inherit signal handling
				or are they killed by SIGINT?
	ERROR: When minishell is still executing a command, 2 prompts are printed
		*/

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		if (g_signal[0] == -1)
		{
			rl_redisplay();
			g_signal[1] = 1;
		}
		else
			g_signal[1] = 130;
	}
	if (sig == SIGQUIT)
	{
		if (g_signal[0] != -1)
		{
			rl_replace_line("", 0);
			write(1, "Quit: 3\n", 8);
			g_signal[1] = 131;
		}
		rl_redisplay();
		write(1, "  \b\b", 4);
	}
}
