/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 14:46:28 by tgrasset          #+#    #+#             */
/*   Updated: 2022/11/10 09:49:54 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_strings(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static int	ft_strlen_till_sep(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static char	*place_part(char *str, char c)
{
	char	*part;
	int		i;

	i = 0;
	part = malloc(sizeof(char) * (ft_strlen_till_sep(str, c) + 1));
	if (!part)
		return (NULL);
	while (str[i] != c && str[i] != '\0')
	{
		part[i] = str[i];
		i++;
	}
	part[i] = '\0';
	return (part);
}

static char	**free_tabs(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		i;

	i = 0;
	split = malloc(sizeof(char *) * (count_strings(s, c) + 1));
	if (!split)
		return (NULL);
	while (*s != '\0')
	{
		while (*s == c && *s != '\0')
			s++;
		if (*s != '\0')
		{
			split[i] = place_part((char *)s, c);
			if (!split[i])
				return (free_tabs(split));
			i++;
		}
		while (*s != c && *s != '\0')
			s++;
	}
	split[i] = NULL;
	return (split);
}
