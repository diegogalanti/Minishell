/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 13:11:12 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/02/06 15:45:31 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
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
		rl_redisplay();
	}
}
