/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tscasso <tscasso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 00:09:03 by tscasso           #+#    #+#             */
/*   Updated: 2023/05/25 22:08:23 by tscasso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <stdio.h>

void my_handler(int signum, siginfo_t *siginfo, void *ucontext)
{
	static int out_char;
	static int pow = 0;

	if (!out_char)
	out_char = 0;
	(void)ucontext;

	// Gestion du signal SIGUSR1
	if (signum == SIGUSR1)
		out_char += 1 << (7 - pow);
	pow += 1;

	// Vérification si tous les bits ont été reçus
	if (pow == 8)
	{
		ft_printf("%c", out_char);
		out_char = 0;
		pow = 0;
		

		// Envoi du signal SIGUSR2 au processus récepteur
		if (kill(siginfo->si_pid, SIGUSR2) == -1)
			handle_error("Signal error!");
	}
}

int	main(void)
{
	struct sigaction	sig;

	ft_printf("Serveur PID: %d\n", getpid());
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = my_handler;
	if ((sigaction(SIGUSR1, &sig, 0)) == -1)
		handle_error("Signal error!");
	if ((sigaction(SIGUSR2, &sig, 0)) == -1)
		handle_error("Signal error!");
	while (1)
		pause();
	return (0);
}