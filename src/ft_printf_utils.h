#ifndef FT_PRINTF_UTILS_H
# define FT_PRINTF_UTILS_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ft_printf.h>

char *ft_strdup(const char *s);
char *ft_lstrdup(const int *s);
char *ft_strcpy(char *dst, const char *src);
void ft_substr(char **res, unsigned int i);
char *ft_strnew(size_t size);
void ft_bzero(void *params, size_t len);
int ft_strlen(const char *s);
void *ft_memalloc(size_t size);
int printf_putchar(const char **c);
int printf_print_percent(const char **s);
int printf_print(const char *s);
char *uint_to_string(unsigned long long int n, t_printf_params params);
char *int_to_string(long long n, t_printf_params params);
void parse_flags(t_printf_params *params, const char **format);
void parse_width(t_printf_params *params, const char **format, va_list args);
void parse_precision(t_printf_params *params, const char **format);
void parse_type(t_printf_params *params, const char **format);
char *print_to_str(t_printf_params params, va_list args);
void add_padding(char **str, t_printf_params params);
int print_with_params(t_printf_params params, va_list args);
int printf_special(const char **format, va_list args);

#endif