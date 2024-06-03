/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 17:22:44 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/01 09:46:12 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	getenv_malloc_size(char **env, int i, int j)
{
	int	k;

	k = 0;
	while (env[i][j] != '\0')
	{
		j++;
		k++;
	}
	return (k);
}

char	*ft_getenv(char *var_name, char **env)
{
	int		i;
	int		j;
	int		k;
	char	*res;

	i = 0;
	while (env[i] != NULL
		&& !(ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0
			&& env[i][ft_strlen(var_name)] == '='))
		i++;
	if (env[i] == NULL)
		return (NULL);
	j = ft_strlen(var_name) + 1;
	k = 0;
	res = malloc(sizeof(char) * (getenv_malloc_size(env, i, j) + 1));
	if (res == NULL)
		return (NULL);
	while (env[i][j] != '\0')
	{
		res[k] = env[i][j];
		j++;
		k++;
	}
	res[k] = '\0';
	return (res);
}

int	var_number(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	return (i);
}

char	**setup_env(char **envp)
{
	int		i;
	char	**res;

	i = 0;
	res = malloc(sizeof(char *) * (var_number(envp) + 1));
	if (res == NULL)
	{
		ft_putendl_fd("Malloc error", 2);
		exit (-1);
	}
	while (envp[i] != NULL)
	{
		res[i] = ft_strdup(envp[i]);
		if (res[i] == NULL)
		{
			free_lex(res);
			ft_putendl_fd("Malloc error", 2);
			exit (-1);
		}
		i++;
	}
	res[i] = NULL;
	return (res);
}
