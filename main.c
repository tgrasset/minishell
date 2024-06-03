/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 15:29:58 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/21 14:19:37 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ret_val = 0;

void	sig_handler_command(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		g_ret_val = 130;
	}
	if (signum == SIGQUIT)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
		g_ret_val = 131;
	}
}

void	sig_handler_prompt(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_ret_val = 130;
	}
	if (signum == SIGQUIT)
	{
		printf("\33[2K\r");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	lex_parse_execute_free(t_sh *sh, char **env)
{
	lexing(sh);
	parsing(sh);
	sh->stdin_save = dup(0);
	sh->interrupted_heredoc = 0;
	if (sh->stdin_save < 0)
		ft_error(sh, 3);
	if (sh->comm != NULL)
	{
		execution(sh, env);
		wait_for_children(sh);
		signal(SIGINT, sig_handler_prompt);
		signal(SIGQUIT, sig_handler_prompt);
	}
	free_all(sh);
	if (dup2(sh->stdin_save, 0) < 0)
		ft_error(sh, 3);
	close(sh->stdin_save);
}

void	quit_with_sigquit(t_sh *sh)
{
	rl_clear_history();
	free_all(sh);
	free_lex(sh->env);
	ft_putendl_fd("exit", 2);
	exit(g_ret_val);
}

int	main(int ac, char **av, char **envp)
{
	t_sh	sh;

	(void)av;
	(void)ac;
	g_ret_val = 0;
	sh.env = setup_env(envp);
	sh.comm = NULL;
	sh.lex = NULL;
	signal(SIGINT, sig_handler_prompt);
	signal(SIGQUIT, sig_handler_prompt);
	signal(SIGTSTP, SIG_IGN);
	while (1)
	{
		sh.buf = readline("\1\033[0;32m\2minishell $> \1\033[0;m\2");
		if (sh.buf == NULL)
			quit_with_sigquit(&sh);
		if (sh.buf[0] != '\0')
		{
			add_history(sh.buf);
			lex_parse_execute_free(&sh, sh.env);
		}
	}
	return (0);
}
