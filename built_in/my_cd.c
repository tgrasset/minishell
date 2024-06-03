/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 15:35:44 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/02 17:22:21 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_ret_val;

void	update_pwd_vars(t_sh *sh, char *dir)
{
	char	*var;

	if (dir == NULL)
		return ;
	var = ft_getenv("OLDPWD", sh->env);
	if (var == NULL)
		cd_add_env_var(sh, "OLDPWD", dir);
	else
	{
		cd_replace_env_var(sh, "OLDPWD", dir);
		free(var);
	}
	dir = NULL;
	dir = getcwd(dir, 0);
	if (dir == NULL)
		return ;
	var = ft_getenv("PWD", sh->env);
	if (var == NULL)
		cd_add_env_var(sh, "PWD", dir);
	else
	{
		cd_replace_env_var(sh, "PWD", dir);
		free(var);
	}
}

void	cd_exit_child_process(t_sh *sh, char *current_dir)
{
	free_lex(sh->env);
	free_all(sh);
	if (current_dir != NULL)
		free(current_dir);
	exit (0);
}

void	change_directory(t_sh *sh, char *dest, int forked, char *current_dir)
{
	struct stat	buf;

	if (stat(dest, &buf) < 0 && errno == EACCES)
		cd_error(sh, dest, forked, 3);
	else if (access(dest, F_OK) != 0)
		cd_error(sh, dest, forked, 1);
	else if (S_ISDIR(buf.st_mode))
	{
		current_dir = getcwd(current_dir, 0);
		if (chdir(dest) == -1)
		{
			if (current_dir != NULL)
				free(current_dir);
			cd_error(sh, dest, forked, 3);
			return ;
		}
		if (forked == 1)
			cd_exit_child_process(sh, current_dir);
		update_pwd_vars(sh, current_dir);
		g_ret_val = 0;
		return ;
	}
	else
		cd_error(sh, dest, forked, 2);
}

void	go_to_home_dir(t_sh *sh, int forked, char *current_dir)
{
	char	*home;

	home = ft_getenv("HOME", sh->env);
	if (home == NULL)
	{
		ft_putendl_fd("msh: cd: HOME not set", 2);
		if (forked == 1)
		{
			free_lex(sh->env);
			free_all(sh);
			exit (1);
		}
		g_ret_val = 1;
		return ;
	}
	change_directory(sh, home, forked, current_dir);
	free(home);
}

void	my_cd(t_sh *sh, t_comm *cmd, int forked)
{
	int		arg_nb;
	char	*current_dir;

	current_dir = NULL;
	arg_nb = check_cd_arg(cmd->argv);
	if (arg_nb > 2)
	{
		ft_putendl_fd("msh: cd: too many arguments", 2);
		if (forked == 1)
		{
			free_lex(sh->env);
			free_all(sh);
			exit (1);
		}
		g_ret_val = 1;
		return ;
	}
	else if (arg_nb == 1)
		go_to_home_dir(sh, forked, current_dir);
	else
		change_directory(sh, cmd->argv[1], forked, current_dir);
}
