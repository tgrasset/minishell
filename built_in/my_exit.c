/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:30:44 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/08 10:20:44 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_ret_val;

int	is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
	{
		i++;
		if (arg[i] == '\0')
			return (0);
	}
	while (arg[i] != '\0')
	{
		if (ft_isdigit(arg[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

void	exit_non_numeric_error(t_sh *sh, char *arg, int forked)
{
	if (forked == 1)
	{
		ft_putstr_fd("msh: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_all(sh);
		exit (2);
	}
	printf("exit\n");
	ft_putstr_fd("msh: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	g_ret_val = 2;
}

int	exit_arg_check(t_sh *sh, t_comm *cmd, int forked)
{
	if (cmd->argv[1] != NULL && cmd->argv[2] != NULL)
	{
		if (forked == 1)
		{
			ft_putendl_fd("msh: exit: too many arguments", 2);
			free_all(sh);
			exit (1);
		}
		printf("exit\n");
		ft_putendl_fd("msh: exit: too many arguments", 2);
		g_ret_val = 1;
		return (1);
	}
	if (cmd->argv[1] != NULL && is_numeric(cmd->argv[1]) == 0)
	{
		exit_non_numeric_error(sh, cmd->argv[1], forked);
		return (1);
	}
	return (0);
}

void	my_exit(t_sh *sh, t_comm *cmd, int forked)
{
	int	ret;

	ret = 0;
	if (exit_arg_check(sh, cmd, forked) != 0)
		return ;
	if (cmd->argv[1] != NULL)
		ret = ft_atoi(cmd->argv[1]);
	if (forked == 0)
	{
		ft_putendl_fd("exit", 2);
		close (sh->stdin_save);
		rl_clear_history();
	}
	free_lex(sh->env);
	free_all(sh);
	exit (ret);
}
