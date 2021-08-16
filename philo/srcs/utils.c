/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 17:39:44 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/16 17:41:13 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** The isdigit() function tests for a decimal digit character.
*/
static int	ft_isdigit(const char c)
{
	return (c >= '0' && c <= '9');
}

/*
** The ft_atoi() function converts the initial portion of the string pointed
** to by str to int representation.
*/
int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (*(str + i) == ' ' || (*(str + i) >= '\t' && *(str + i) <= '\r'))
		i++;
	if (*(str + i) == '+' || *(str + i) == '-')
		if (*(str + i++) == '-')
			sign = -1;
	while (ft_isdigit(*(str + i)))
	{
		if (result * sign > INT32_MAX)
			return (-1);
		else if (result * sign < INT32_MIN)
			return (0);
		result = result * 10 + (*(str + i++) - '0');
	}
	result *= sign;
	return (result);
}

/*
** copy memory area values to another area
*/
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	i = 0;
	while (i < n)
	{
		s = (char *) src + i;
		d = (char *) dest + i;
		*d = *s;
		i++;
	}
	return (dest);
}
