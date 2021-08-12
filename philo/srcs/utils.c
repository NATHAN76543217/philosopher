#include "philo.h"

void		log_philo(char *str, const t_philo *philo)
{
	printf("%ld %3d  %s\n", elapsedStart(*(philo->timestamp)), philo->id, str);	
}

void		log_simu(char *str, const t_philo_simu *simu)
{
	printf("%ld simu %s\n", elapsedStart(simu->timestamp), str);	
}

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
int			ft_atoi(const char *str)
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
		(*(str + i++) == '-') ? sign = -1 : 0;
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
** The error_msg() function print a given error before returning a given value 
*/
int			error_msg(const char *const str, const int ret_value)
{
	printf("%s", str);
	return ret_value;
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
	return dest;
}