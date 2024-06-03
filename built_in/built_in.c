/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 10:39:32 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/01 17:01:12 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_built_in(t_sh *sh, t_comm *cmd, char **env, int forked)
{
	if (ft_strncmp(cmd->file, "echo", 5) == 0)
		my_echo(sh, cmd);
	else if (ft_strncmp(cmd->file, "cd", 3) == 0)
		my_cd(sh, cmd, forked);
	else if (ft_strncmp(cmd->file, "pwd", 4) == 0)
		my_pwd(sh);
	else if (ft_strncmp(cmd->file, "export", 7) == 0)
		my_export(sh, cmd, forked);
	else if (ft_strncmp(cmd->file, "unset", 6) == 0)
		my_unset(sh, cmd, forked);
	else if (ft_strncmp(cmd->file, "env", 4) == 0)
		my_env(sh, cmd, env);
	else if (ft_strncmp(cmd->file, "exit", 5) == 0)
		my_exit(sh, cmd, forked);
}

int	env_built_in(t_comm *cmd)
{
	if (cmd->file == NULL || cmd->file[0] == '\0')
		return (1);
	if (ft_strncmp(cmd->file, "cd", 3) == 0
		|| ft_strncmp(cmd->file, "export", 7) == 0
		|| ft_strncmp(cmd->file, "unset", 6) == 0
		|| ft_strncmp(cmd->file, "exit", 5) == 0)
		return (0);
	return (1);
}
