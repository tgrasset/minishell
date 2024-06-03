/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:54:08 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/03 13:32:54 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var_name(t_sh *sh, char *arg)
{
	int		i;
	char	*res;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	res = malloc(sizeof(char) * (i + 1));
	if (res == NULL)
		ft_error(sh, 1);
	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
	{
		res[i] = arg[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_new_value(t_sh *sh, char *arg, char *name)
{
	int		i;
	int		j;
	char	*res;

	res = malloc(sizeof(char) * (ft_strlen(arg) - ft_strlen(name) + 1));
	if (res == NULL)
	{
		free(name);
		ft_error(sh, 1);
	}
	i = 0;
	j = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	if (arg[i] == '=')
		i++;
	while (arg[i] != '\0')
	{
		res[j] = arg[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
}

void	copy_new_var(char *new, char *name, char *value)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (name[i] != '\0')
	{
		new[i] = name[i];
		i++;
	}
	new[i] = '=';
	i++;
	while (value[j] != '\0')
	{
		new[i] = value[j];
		i++;
		j++;
	}
	new[i] = '\0';
	free(value);
}

void	replace_env(t_sh *sh, char **newenv, char *new)
{
	int	i;

	i = 0;
	while (sh->env[i] != NULL)
	{
		newenv[i] = ft_strdup(sh->env[i]);
		if (newenv[i] == NULL)
		{
			free(new);
			free_lex(newenv);
			ft_error(sh, 1);
		}
		i++;
	}
	newenv[i] = new;
	newenv[i + 1] = NULL;
	free_lex(sh->env);
	sh->env = newenv;
}

void	finish_forked_export(t_sh *sh, int forked, int ret_value)
{
	if (forked == 1)
	{
		free_lex(sh->env);
		free_all(sh);
		exit (ret_value);
	}
}
