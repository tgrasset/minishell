/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 20:49:07 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/06 16:04:38 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_ret_val;

void	wait_for_children(t_sh *sh)
{
	t_comm	*tmp;
	int		status;

	status = 0;
	tmp = sh->comm;
	if (g_ret_val == 130 && sh->interrupted_heredoc == 1)
		return ;
	signal(SIGINT, sig_handler_command);
	signal(SIGQUIT, sig_handler_command);
	while (tmp != NULL)
	{
		if (tmp->pid != -42)
		{
			if (waitpid(tmp->pid, &status, 0) < 0)
				ft_error(sh, 7);
		}
		if (g_ret_val == 130 || g_ret_val == 131)
			return ;
		if (tmp->next == NULL && tmp->in_out_fail == 0
			&& is_unpiped_env_builtin(tmp) == 0)
			g_ret_val = WEXITSTATUS(status);
		tmp = tmp->next;
	}
}

char	**split_paths(t_sh *sh, char *temp, char **paths, char *file)
{
	temp = ft_getenv("PATH", sh->env);
	if (temp == NULL)
		command_error(sh, file, 2, NULL);
	paths = ft_split(temp, ':');
	free(temp);
	if (paths == NULL)
		ft_error(sh, 1);
	return (paths);
}

void	check_if_dir(t_sh *sh, char *path, char *cmd)
{
	struct stat	buf;

	if (stat(path, &buf) < 0)
		ft_error(sh, 6);
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Is a directory", 2);
		free_lex(sh->env);
		free_all(sh);
		if (path != NULL)
			free(path);
		exit (126);
	}
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": Execve failed", 2);
	if (path != NULL)
		free(path);
	free_lex(sh->env);
	free_all(sh);
	exit (126);
}

void	check_no_such_file(t_sh *sh, char *path, char *cmd)
{
	if (ft_strchr(cmd, '/') == NULL)
		return ;
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": No such file or directory", 2);
	if (path != NULL)
		free(path);
	free_lex(sh->env);
	free_all(sh);
	exit (127);
}

void	command_error(t_sh *sh, char *cmd, int num, char *path)
{
	if (num == 1 || num == 2)
	{
		if (num == 2)
			check_no_such_file(sh, path, cmd);
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(cmd, 2);
		if (num == 1)
			ft_putendl_fd(": Permission denied", 2);
		else if (num == 2)
			ft_putendl_fd(": command not found", 2);
		if (path != NULL)
			free(path);
		free_lex(sh->env);
		free_all(sh);
		if (num == 1)
			exit (126);
		else if (num == 2)
			exit (127);
	}
	check_if_dir(sh, path, cmd);
}
