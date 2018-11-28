#include "ft_printf_utils.h"

char *print_to_str(t_printf_params params, va_list args)
{
	char *s;

	if (params.type == STRING)
	{
		if (params.modifier == LONG)
			s = ft_lstrdup(va_arg(args, int*));
		else
		s = ft_strdup(va_arg(args, char*));
		if (params.precision != -1 && params.precision < ft_strlen(s))
			s[params.precision] = 0;
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
		s = ft_strnew(0);
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
	padchar = (char)(params.pad_zero && params.precision == -1 ? '0' : ' ');
	padding = params.margin - i;
	if (padding <= 0)
		return;
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
	char *ptr;
	char to_print;
	int ret;

	if (params.type == CHARACTER)
	{
		str = ft_strnew(1);
		*str = -1;
	}
	else
		str = print_to_str(params, args);
	add_padding(&str, params);
	if (params.type == CHARACTER)
	{
		ptr = str;
		ret = 0;
		while (str[ret])
		{
			if (str[ret] == -1)
			{
				if (ret == 0)
					str++;
				else
					str[ret] = 0;
				break;
			}
			ret++;
		}
		to_print = (char)va_arg(args, int);
		if (params.justify_left)
		{
			write(1, &to_print, 1);
			ret = printf_print(str) + 1;
		}
		else
		{
			ret = printf_print(str) + 1;
			write(1, &to_print, 1);
		}
		free(ptr);
	}
	else
	{
		ret = printf_print(str);
		free(str);
	}
	return ret;
}

int printf_special(const char **format, va_list args)
{
	t_printf_params params;

	ft_bzero(&params, sizeof(params));
	params.precision = -1;
	parse_flags(&params, format);
	parse_width(&params, format, args);
	parse_precision(&params, format);
	parse_type(&params, format);
	return print_with_params(params, args);
}