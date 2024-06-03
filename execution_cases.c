/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cases.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 14:10:03 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/03 12:59:26 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_in_pipe(t_sh *sh, int *pipe_fd, t_comm *cmd, char **env)
{
	if (pipe(pipe_fd) < 0)
		ft_error(sh, 4);
	exec_command(cmd, sh, pipe_fd, env);
}

void	pipe_0(t_sh *sh, int *pipe_fd, t_comm *cmd)
{
	if (pipe(pipe_fd) < 0)
		ft_error(sh, 4);
	cmd->pid = fork();
	if (cmd->pid < 0)
		ft_error(sh, 5);
	else if (cmd->pid == 0)
	{
		close(cmd->stdout_save);
		close(sh->stdin_save);
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], 1) < 0)
			ft_error(sh, 3);
		close(pipe_fd[1]);
		write(1, "", 0);
		free_lex(sh->env);
		free_all(sh);
		exit(0);
	}
	else if (cmd->pid != 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], 0) < 0)
			ft_error(sh, 3);
		close(pipe_fd[0]);
	}
}

void	exec_outfile_pipe_0(t_sh *sh, int *pipe_fd, t_comm *cmd, char **env)
{
	if (cmd->file == NULL || cmd->file[0] == '\0')
		return ;
	cmd->pid = fork();
	if (cmd->pid < 0)
		ft_error(sh, 5);
	else if (cmd->pid == 0)
		exec_command_2(cmd, sh, env);
	else
		pipe_0(sh, pipe_fd, cmd);
}

int	is_unpiped_env_builtin(t_comm *cmd)
{
	if (cmd != NULL && cmd->next == NULL)
	{
		if (cmd->file == NULL || cmd->file[0] == '\0')
			return (0);
		if (ft_strncmp(cmd->file, "cd", 3) == 0
			|| ft_strncmp(cmd->file, "export", 7) == 0
			|| ft_strncmp(cmd->file, "unset", 6) == 0
			|| ft_strncmp(cmd->file, "exit", 5) == 0)
			return (1);
	}
	return (0);
}

void	choose_exec_case(t_sh *sh, t_comm *cmd, int *pipe_fd, char **env)
{
	if (cmd->next == NULL && env_built_in(cmd) == 0)
	{
		check_built_in(sh, cmd, env, 0);
		return ;
	}
	else if (cmd->outfile == 1 && cmd->next != NULL)
		exec_outfile_pipe_0(sh, pipe_fd, cmd, env);
	else if (cmd->outfile == 0 && cmd->next != NULL)
		exec_in_pipe(sh, pipe_fd, cmd, env);
	else
		exec_command(cmd, sh, NULL, env);
	close(1);
	if (dup2(cmd->stdout_save, 1) < 0)
		ft_error(sh, 3);
	close(cmd->stdout_save);
}
