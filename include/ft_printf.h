#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>


int ft_printf(const char *format, ...);

enum printf_types {
	CHARACTER,
	STRING,
	INTEGER,
	FLOAT,
	NONE,
};

enum printf_modifiers {
	DEFAULT,
	LONG_LONG,
	LONG,
	SHORT,
	CHAR,
};

typedef struct s_printf_params
{
	char justify_left;
	char pad_zero;
	char force_sign;
	char pad_positive_blank;
	int margin;
	char base_prefix;
	int precision;
	int base;
	char no_sign;
	enum printf_types type;
	enum printf_modifiers modifier;
	char uppercase;
}				t_printf_params;

#endif