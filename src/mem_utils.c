#include "ft_printf_utils.h"


void ft_bzero(void *params, size_t len)
{
	while (len)
		((char *)params)[--len] = 0;
}

int ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return i;
}

void *ft_memalloc(size_t size)
{
	void *ptr;

	if (!(ptr = malloc(size)))
		return (NULL);
	ft_bzero(ptr, size);
	return (ptr);
}