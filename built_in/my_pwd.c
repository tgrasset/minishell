/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 16:00:27 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/01 16:50:58 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	my_pwd(t_sh *sh)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (cwd == NULL)
	{
		free_lex(sh->env);
		free_all(sh);
		ft_putendl_fd("msh: pwd: Cannot get current woking directory", 2);
		exit (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	free_lex(sh->env);
	free_all(sh);
	exit (0);
}
