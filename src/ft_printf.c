#include <ft_printf.h>
#include <stdlib.h>
#include <ctype.h>
#include "ft_printf_utils.h"

int ft_printf(const char *format, ...)
{
	va_list args;
	int len;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '%')
				len += printf_putchar(&format);
			else
				len += printf_special(&format, args);
		}
		else
			len += printf_print_percent(&format);
	}
	va_end(args);
	return len;
}