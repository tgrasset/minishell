/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 11:35:16 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/01 16:00:41 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_echo_option(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i] != NULL)
	{
		j = 1;
		if (argv[i][0] == '-')
		{
			while (argv[i][j] != '\0' && argv[i][j] == 'n')
				j++;
			if (argv[i][j] == '\0')
				i++;
			else
				return (i - 1);
		}
		else
			return (i - 1);
	}
	return (i - 1);
}

void	my_echo(t_sh *sh, t_comm *cmd)
{
	int	no_nl;
	int	i;

	no_nl = check_echo_option(cmd->argv);
	i = no_nl + 1;
	while (cmd->argv[i] != NULL)
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (no_nl == 0)
		ft_putchar_fd('\n', 1);
	free_lex(sh->env);
	free_all(sh);
	exit (0);
}
