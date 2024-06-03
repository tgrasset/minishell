/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:54:24 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/03 13:42:51 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_ret_val;

void	my_env(t_sh *sh, t_comm *cmd, char **env)
{
	int	i;

	if (cmd->argv[1] != NULL)
	{
		ft_putendl_fd("msh: env: only works without arguments", 2);
		free_all(sh);
		exit (1);
	}
	i = 0;
	while (env[i] != NULL)
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	free_lex(sh->env);
	free_all(sh);
	exit (0);
}

int	invalid_var_name(t_sh *sh, char *arg, int forked)
{
	ft_putstr_fd("msh: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	finish_forked_export(sh, forked, 1);
	g_ret_val = 1;
	return (1);
}
