/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgrasset <tgrasset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:20:30 by tgrasset          #+#    #+#             */
/*   Updated: 2023/03/06 15:49:49 by tgrasset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_malloc_size(long int n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n > 0)
	{
		size++;
		n = n / 10;
	}
	return (size);
}

static char	*strrev(char *str)
{
	char	temp;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (i < len / 2)
	{
		temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
		i++;
	}
	return (str);
}

static char	*place_digits(char *str, long int n, int sign)
{
	int	i;

	i = 0;
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	while (n > 0)
	{
		str[i] = n % 10 + 48;
		n = n / 10;
		i++;
	}
	if (sign == -1)
	{
		str[i] = '-';
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char		*res;
	int			sign;
	long int	nb;

	nb = n;
	sign = 1;
	res = malloc(sizeof(char) * (get_malloc_size(nb) + 1));
	if (!res)
		return (NULL);
	if (nb < 0)
	{
		nb = -nb;
		sign = -1;
	}
	res = place_digits(res, nb, sign);
	return (strrev(res));
}
