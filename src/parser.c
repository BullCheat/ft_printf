#include "ft_printf_utils.h"

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

char ft_tolower(char c)
{
	if ('A' <= c && c <= 'Z')
		return (char)(c + 32);
	else
		return c;
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
	c = ft_tolower(**format); // TODO
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
			params->base_prefix = (char)(1 + (c == 'p'));
			params->modifier = LONG_LONG;
		}
	}
	else if (c == 's')
		params->type = STRING;
	else if (c == 'c')
		params->type = CHARACTER;
	else if (c == 'f')
		params->type = FLOAT;
	if (c == 'd' && params->uppercase)
		params->modifier = LONG;
}