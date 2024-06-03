/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 16:55:35 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/02 18:43:52 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_val;

int	is_last_redir(t_redir *redir)
{
	t_redir	*temp;

	temp = redir->next;
	while (temp != NULL)
	{
		if (temp->output == redir->output)
			return (0);
		temp = temp->next;
	}
	return (1);
}

int	input_file_check(t_sh *sh, t_redir *redir)
{
	if (access(redir->name, F_OK) != 0)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(redir->name, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (1);
	}
	else if (access(redir->name, R_OK) != 0)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(redir->name, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (1);
	}
	if (is_last_redir(redir))
	{
		redir->fd = open(redir->name, O_RDONLY);
		if (redir->fd < 0)
			ft_error(sh, 2);
		if (dup2(redir->fd, 0) < 0)
			ft_error(sh, 3);
		close(redir->fd);
	}
	return (0);
}

int	output_file_create(t_sh *sh, t_redir *redir, t_comm *cmd)
{
	if (check_if_empty_str(redir->name) != 0)
		return (1);
	if (redir->doubl == 0)
		redir->fd = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		redir->fd = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (redir->fd < 0)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(redir->name, 2);
		ft_putendl_fd(": Permission denied", 2);
		return (1);
	}
	if (is_last_redir(redir) && is_unpiped_env_builtin(cmd) == 0)
	{
		if (cmd->file != NULL && dup2(redir->fd, 1) < 0)
			ft_error(sh, 3);
	}
	close(redir->fd);
	return (0);
}

int	check_output_redir(t_comm *cmd, t_sh *sh)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir != NULL)
	{
		if (redir->output == 1 && output_file_create(sh, redir, cmd) != 0)
		{
			close_fds(cmd);
			close(cmd->stdout_save);
			cmd->in_out_fail = 1;
			g_ret_val = 1;
			return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	redirections(t_comm *cmd, t_sh *sh)
{
	t_redir	*redir;

	if (cmd->redir == NULL)
		return (0);
	redir = cmd->redir;
	while (redir != NULL)
	{
		if (redir->doubl == 0 && redir->output == 0)
		{
			if (input_file_check(sh, redir) != 0)
			{
				close_fds(cmd);
				close(cmd->stdout_save);
				cmd->in_out_fail = 1;
				g_ret_val = 1;
				return (1);
			}
		}
		else if (redir->doubl == 1 && redir->output == 0)
			here_doc(sh, redir);
		redir = redir->next;
	}
	if (g_ret_val == 130 || check_output_redir(cmd, sh) != 0)
		return (1);
	return (0);
}
