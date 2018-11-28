#include "ft_printf_utils.h"

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

char get_digit(long long int i, t_printf_params params)
{
	if (i < 10)
		return (char)('0' + i);
	else if (params.uppercase)
		return (char)('A' + i - 10);
	else
		return (char)('a' + i - 10);
}

char *uint_to_string(unsigned long long int n, t_printf_params params)
{
	int len;
	int tmp;
	int start;
	unsigned long long copy;
	char *res;
	char should_prefix;

	copy = n;
	len = params.pad_positive_blank;
	should_prefix = params.base_prefix && params.base % 8 == 0 &&
					(n != 0 || (params.base_prefix & 2));
	if (should_prefix)
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
	if (should_prefix)
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
	if (should_prefix && res[1] == '0' && ft_strlen(res) > params.precision)
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

	if (n == (long long)0x8000000000000000)
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