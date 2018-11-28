#include <ft_printf.h>
#include <printf.h>

int main() {
#define str "|%52lc|\n"
#define a1 25
	ft_printf(str, a1);
	printf(str, a1);
}