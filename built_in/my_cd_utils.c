/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 16:50:21 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/02 17:25:02 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_ret_val;

void	cd_replace_env_var(t_sh *sh, char *name, char *value)
{
	int		i;
	char	*new;

	i = 0;
	while (sh->env[i] != NULL
		&& !(ft_strncmp(sh->env[i], name, ft_strlen(name)) == 0
			&& sh->env[i][ft_strlen(name)] == '='))
		i++;
	new = malloc(sizeof(char) * (ft_strlen(name) + ft_strlen(value) + 2));
	if (new == NULL)
	{
		free(value);
		ft_error(sh, 1);
	}
	free(sh->env[i]);
	copy_new_var(new, name, value);
	sh->env[i] = new;
}

void	cd_add_env_var(t_sh *sh, char *name, char *value)
{
	char	*new;
	char	**newenv;
	int		i;

	new = malloc(sizeof(char) * (ft_strlen(name) + ft_strlen(value) + 2));
	if (new == NULL)
	{
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
		free(value);
		ft_error(sh, 1);
	}
	replace_env(sh, newenv, new);
}

int	check_cd_arg(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	return (i);
}

void	cd_error(t_sh *sh, char *dest, int forked, int type)
{
	if (type == 1)
	{
		ft_putstr_fd("msh: cd : ", 2);
		ft_putstr_fd(dest, 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	else if (type == 2)
	{
		ft_putstr_fd("msh: cd : ", 2);
		ft_putstr_fd(dest, 2);
		ft_putendl_fd(": Not a directory", 2);
	}
	else if (type == 3)
	{
		ft_putstr_fd("msh: cd : ", 2);
		ft_putstr_fd(dest, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	if (forked == 1)
	{
		free_lex(sh->env);
		free_all(sh);
		exit (1);
	}
	g_ret_val = 1;
}
