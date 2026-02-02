#include "chaos.h"
/* 
long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
} */

void	print_time(long start, long end, char *msg)
{
	double	duration;

	duration = (double)(end - start) / 1000;
	printf(msg, duration);
}