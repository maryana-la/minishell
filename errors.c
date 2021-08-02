//
// Created by Jamar Jacquelynn on 7/6/21.
//

#include "minishell.h"

void error_handler(char *arg, int errorcode)
{
	if (errorcode == 1)
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return;
	}
	if (errorcode == 2)
	{
		write(2, "minishell: unset: `", 19);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return;
	}
}
