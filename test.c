#include <stdio.h>
#include "libft/libft.h"

int main()
{
	char *a = "QWEDFRgfbrREG";
	char b[100];
	int i;

	i = 0;

	while (a[i] != 0)
	{
		b[i] = ft_tolower(a[i]);
		i++;
	}
	printf("%s\n", b);
	return 0;
}