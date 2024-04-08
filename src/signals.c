/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:11:12 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/04/08 11:26:55 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal[2];

/* set_signal: This function handles the signals SIGINT (Ctrl-C)
				and SIGQUIT (Ctrl-\) with sigaction which calls
				the function handle signals.
	SIQUIT is ignored 
	SIGINT should result in displaying a new prompt line. 
	QUESTION: Is error handling needed?*/

int	set_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signals;
	sa.sa_flags = 0; // valgrind throws error otherwise; check if functions as should
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	return (0);
}

/* handle_signals: handles SIGINT and SIGQUIT (more signals could be added)
	SIGINT: a new prompt line is displayed:
	-rl_replace_line empties readline line buffer
		so that the previous user input is not printed
	- a new line is printed
@ Diego: could you explain the g_signal?
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

void	reset_g_signal(void)
{
	g_signal[0] = -1;
	g_signal[1] = -1;
}
