/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:52:50 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/08 10:33:16 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_ret_val;

int	check_export_cmd_format(t_sh *sh, t_comm *cmd, int forked)
{
	int	i;

	i = 0;
	while (cmd->argv[i] != NULL)
		i++;
	if (i == 1)
	{
		ft_putendl_fd("msh: export: Usage : export name=[value]", 2);
		if (forked == 1)
		{
			free_lex(sh->env);
			free_all(sh);
			exit (1);
		}
		g_ret_val = 1;
		return (1);
	}
	return (0);
}

int	check_export_var_name(t_sh *sh, char *arg, int forked, int name_ok)
{
	int	i;

	i = 0;
	if (arg[0] == '=' || arg[0] == '\0')
		name_ok = 0;
	while (name_ok == 1 && arg[i] != '\0' && arg[i] != '=')
	{
		if ((ft_isalpha(arg[0]) == 0 && arg[0] != '_')
			|| (i > 0 && ft_isalnum(arg[i]) == 0 && arg[i] != '_'))
			name_ok = 0;
		i++;
	}
	if (name_ok == 1 && arg[i] == '\0')
	{
		finish_forked_export(sh, forked, 0);
		g_ret_val = 0;
		return (1);
	}
	if (name_ok == 0)
		return (invalid_var_name(sh, arg, forked));
	return (0);
}

void	replace_existing_var(t_sh *sh, char *arg, char *name, char *val)
{
	int		i;
	char	*new;

	i = 0;
	while (sh->env[i] != NULL
		&& !(ft_strncmp(sh->env[i], name, ft_strlen(name)) == 0
			&& sh->env[i][ft_strlen(name)] == '='))
		i++;
	val = get_new_value(sh, arg, name);
	new = malloc(sizeof(char) * (ft_strlen(name) + ft_strlen(val) + 2));
	if (new == NULL)
	{
		free(name);
		free(val);
		ft_error(sh, 1);
	}
	free(sh->env[i]);
	copy_new_var(new, name, val);
	free(name);
	sh->env[i] = new;
}

void	add_new_env_var(t_sh *sh, char *arg, char *name, char *value)
{
	char	*new;
	char	**newenv;
	int		i;

	value = get_new_value(sh, arg, name);
	new = malloc(sizeof(char) * (ft_strlen(name) + ft_strlen(value) + 2));
	if (new == NULL)
	{
		free(name);
		free(value);
		ft_error(sh, 1);
	}
	copy_new_var(new, name, value);
	i = 0;
	while (sh->env[i] != NULL)
		i++;
	newenv = malloc(sizeof(char *) * (i + 2));
	if (newenv == NULL)
	{
		free(name);
		free(value);
		ft_error(sh, 1);
	}
	free(name);
	replace_env(sh, newenv, new);
}

void	my_export(t_sh *sh, t_comm *cmd, int forked)
{
	char	*name;
	char	*value;
	int		i;

	value = NULL;
	if (check_export_cmd_format(sh, cmd, forked) == 1)
		return ;
	i = 0;
	while (cmd->argv[++i] != NULL)
	{
		if (check_export_var_name(sh, cmd->argv[i], forked, 1) == 1)
			continue ;
		name = get_var_name(sh, cmd->argv[i]);
		value = ft_getenv(name, sh->env);
		if (value != NULL)
		{
			free(value);
			replace_existing_var(sh, cmd->argv[i], name, value);
		}
		else
			add_new_env_var(sh, cmd->argv[i], name, value);
	}
	finish_forked_export(sh, forked, 0);
	g_ret_val = 0;
}
