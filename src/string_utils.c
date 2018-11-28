#include "ft_printf_utils.h"

char *ft_strdup(const char *s)
{
	unsigned int i;
	char *ret;

	if (s == NULL)
		return ft_strdup("(null)");
	i = 0;
	while (s[i])
		i++;
	ret = ft_strnew(i);
	ft_strcpy(ret, s);
	return ret;
}

char *ft_lstrdup(const int *s)
{
	unsigned int i;
	char *ret;

	if (s == NULL)
		return ft_strdup("(null)");
	i = 0;
	while (s[i])
		i++;
	ret = ft_strnew(i);
	i = 0;
	while (s[i])
	{
		ret[i] = (char)s[i];
		i++;
	}
	return ret;
}

char *ft_strcpy(char *dst, const char *src)
{
	char *cpy;

	cpy = dst;
	while (*src)
		*cpy++ = *src++;
	*cpy = 0;
	return (dst);
}

char *ft_strnew(size_t size)
{
	return (ft_memalloc(size + 1));
}
