#include "ft_printf_utils.h"

int printf_putchar(const char **c)
{
	write(1, *c, 1);
	*c += 1;
	return 1;
}

int printf_print_percent(const char **s)
{
	unsigned int i;

	i = 0;
	while ((*s)[i] != '\0' && (*s)[i] != '%')
		i++;
	write(1, *s, i);
	*s += i;
	return i;
}

int printf_print(const char *s)
{
	unsigned int i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
	return i;
}