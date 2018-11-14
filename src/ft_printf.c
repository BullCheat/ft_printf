#include <ft_printf.h>
#include <stdlib.h>
#include <ctype.h>

void ft_bzero(void *params, size_t len)
{
	while (len)
		((char *)params)[--len] = 0;
}

int ft_strlen(char *s)
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

char *ft_strnew(size_t size)
{
	return (ft_memalloc(size + 1));
}

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

int printf_print(char *s)
{
	unsigned int i;

	if (s == NULL)
		return printf_print("(null)");
	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
	return i;
}

void parse_flags(t_printf_params *params, const char **format)
{
	char c;

	c = **format;
	while (c == '+' || c == '-' || c == '#' || c == ' ' || c == '0')
	{
		if (c == '+')
			params->force_sign = 1;
		else if (c == '-')
			params->justify_left = 1;
		else if (c == '#')
			params->base_prefix = 1;
		else if (c == ' ')
			params->pad_positive_blank = 1;
		else if (c == '0')
			params->pad_zero = 1;
		*format += 1;
		c = **format;
	}
	if (params->justify_left)
		params->pad_zero = 0;
}

char	*ft_strcpy(char *dst, const char *src)
{
	char *cpy;

	cpy = dst;
	while (*src)
		*cpy++ = *src++;
	*cpy = 0;
	return (dst);
}


int printf_parse_int(const char **s)
{
	int sign = **s == '-' ? -1 : 1;
	int num = 0;

	if (sign == -1)
		*s += 1;
	while (**s && **s >= '0' && **s <= '9')
	{
		num *= 10;
		num += **s - '0';
		*s += 1;
	}
	return num;
}

void parse_width(t_printf_params *params, const char **format, va_list args)
{
	if (**format == '*')
	{
		params->margin = va_arg(args, int);
		*format += 1;
	}
	else
		params->margin = printf_parse_int(format);
	if (**format == '*')
		parse_width(params, format, args);
}

void parse_precision(t_printf_params *params, const char **format)
{
	if (**format == '.')
	{
		*format += 1;
		params->precision = printf_parse_int(format);
	}
}

void parse_type(t_printf_params *params, const char **format)
{
	char c;
	params->type = NONE;
	params->modifier = DEFAULT;
	if (**format == 'h' || **format == 'H')
	{
		*format += 1;
		if (**format == 'h' || **format == 'H')
		{
			*format += 1;
			params->modifier = CHAR;
		}
		else
			params->modifier = SHORT;
	}
	if (**format == 'l' || **format == 'L')
	{
		*format += 1;
		if (**format == 'l' || **format == 'L')
		{
			*format += 1;
			params->modifier = LONG_LONG;
		}
		else
			params->modifier = LONG;
	}
	if (**format == 'z' || **format == 'j' || **format == 't')
	{
		*format += 1;
		params->modifier = LONG_LONG;
	}
	c = (char)tolower(**format); // TODO
	params->uppercase = **format != c;
	*format += 1;
	if (c == 'i' || c == 'd' || c == 'o' || c == 'u' || c == 'x' ||
		c == 'p' || c == 'b')
	{
		params->type = INTEGER;
		params->no_sign = c != 'i' && c != 'd';
		if (c == 'i' || c == 'd' || c == 'u')
			params->base = 10;
		else if (c == 'o')
			params->base = 8;
		else if (c == 'x')
			params->base = 16;
		else if (c == 'b')
			params->base = 2;
		else
		{
			params->base = 16;
			params->base_prefix = 1;
			params->modifier = LONG_LONG;
		}
	}
	else if (c == 's')
		params->type = STRING;
	else if (c == 'c')
		params->type = CHARACTER;
	else if (c == 'f')
		params->type = FLOAT;
}

char get_digit(long long int i, t_printf_params params)
{
	if (i < 10)
		return (char)('0' + i);
	else if (params.uppercase)
		return (char)('A' + i - 10);
	else
		return (char)('a' + i - 10);
}

void ft_substr(char **res, unsigned int i)
{
	char *new;
	unsigned int a;

	a = i;
	while ((*res)[a])
		a++;
	new = ft_strnew((size_t)(a - i));
	a = 0;
	while ((*res)[i])
		new[a++] = (*res)[i++];
	free(*res);
	*res = new;
}

char *uint_to_string(unsigned long long int n, t_printf_params params)
{
	int len;
	int tmp;
	int start;
	unsigned long long copy;
	char *res;

	copy = n;
	len = params.pad_positive_blank;
	if (params.base_prefix && params.base % 8 == 0 && n != 0)
		len += params.base / 8;
	start = len;

	tmp = 1;
	while ((copy /= params.base) > 0)
		tmp++;
	len += tmp < params.precision ? params.precision : tmp;

	if (!(res = ft_strnew((size_t)len--)))
		return (NULL);
	if (params.pad_positive_blank)
		*res = ' ';
	if (params.force_sign && params.base == 10)
		*res = '+';
	if (params.base_prefix && params.base % 8 == 0 && n != 0)
	{
		res[params.force_sign ? 1 : 0] = '0';
		if (params.base == 16)
			res[params.force_sign + 1] = get_digit('x' - 'a' + 10, params);
	}
	while (len >= start)
	{
		res[len] = get_digit(n % params.base, params);
		n /= params.base;
		len--;
	}
	if (params.base == 8 && params.base_prefix && res[1] == '0' && ft_strlen(res) > params.precision)
		ft_substr(&res, 1);
	return res;
}

char *int_to_string(long long n, t_printf_params params)
{
	int len;
	int tmp;
	int start;
	long long copy;
	char *res;

	if (n == (long long) 0x8000000000000000)
		return "-9223372036854775808";
	copy = n > 0 ? n : -n;

	len = (n < 0 || params.force_sign) + (params.pad_positive_blank && n >= 0);
	start = len;

	tmp = 1;
	while ((copy /= params.base) > 0)
		tmp++;
	len += tmp < params.precision ? params.precision : tmp;

	copy = n > 0 ? n : -n;
	if (!(res = ft_strnew((size_t)len--)))
		return (NULL);
	if (n >= 0 && params.pad_positive_blank)
		*res = ' ';
	if (n < 0 || (params.force_sign && params.base == 10))
		*res = (char)(n < 0 ? '-' : '+');
	while (len >= start)
	{
		res[len] = get_digit(copy % params.base, params);
		copy /= params.base;
		len--;
	}
	return res;
}

char *print_to_str(t_printf_params params, va_list args)
{
	char *s;

	if (params.type == STRING)
		return va_arg(args, char*);
	else if (params.type == CHARACTER)
	{
		s = ft_strnew(1);
		s[0] = (char)va_arg(args, int);
	}
	else if (params.type == INTEGER)
	{
		if (params.no_sign)
		{
			unsigned long long n = (unsigned long long int)-1;
			if (params.modifier == LONG_LONG)
				n = va_arg(args, unsigned long long);
			else if (params.modifier == LONG)
				n = va_arg(args, unsigned long);
			else if (params.modifier == DEFAULT)
				n = va_arg(args, unsigned int);
			else if (params.modifier == SHORT)
				n = (unsigned short)va_arg(args, unsigned int);
			else if (params.modifier == CHAR)
				n = (unsigned char)va_arg(args, unsigned int);
			s = uint_to_string(n, params);
		}
		else
		{
			long long n = (long long)-1;
			if (params.modifier == LONG_LONG)
				n = va_arg(args, long long);
			else if (params.modifier == LONG)
				n = va_arg(args, long);
			else if (params.modifier == DEFAULT)
				n = va_arg(args, int);
			else if (params.modifier == SHORT)
				n = (short)va_arg(args, int);
			else if (params.modifier == CHAR)
				n = (char)va_arg(args, int);
			s = int_to_string(n, params);
		}
	}
	else if (params.type == FLOAT)
	{
		s = "TODO float";
	}
	else
		s = "";
	return s;
}

void add_padding(char **str, t_printf_params params)
{
	char padchar;
	int padding;
	int i;
	char *new;

	i = 0;
	while (str[0][i])
		i++;
	padchar = (char)(params.pad_zero && !params.precision ? '0' : ' ');
	padding = params.margin - i;
	if (padding <= 0)
		return ;
	new = ft_strnew((size_t)params.margin);
	if (params.justify_left)
	{
		ft_strcpy(new, *str);
		while (i < params.margin)
			new[i++] = padchar;
	}
	else
	{
		i = 0;
		while (i < padding)
			new[i++] = padchar;
		ft_strcpy(new + padding, *str);
		if (padchar == '0')
		{
			if (**str == '+' || **str == '-' || **str == ' ')
			{
				*new = **str;
				new[padding] = '0';
			}
			else if (**str == '0' && (str[0][1] == 'x' || str[0][1] == 'X'))
			{
				new[1] = str[0][1];
				new[padding + 1] = '0';
			}
		}
	}
	free(*str);
	*str = new;
}

int print_with_params(t_printf_params params, va_list args)
{
	char *str;
	int ret;

	str = print_to_str(params, args);
	add_padding(&str, params);
	ret = printf_print(str);
	free(str);
	return ret;
}

int printf_special(const char **format, va_list args)
{
	t_printf_params params;

	ft_bzero(&params, sizeof(params));
	parse_flags(&params, format);
	parse_width(&params, format, args);
	parse_precision(&params, format);
	parse_type(&params, format);
	return print_with_params(params, args);
}

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