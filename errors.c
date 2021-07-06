//
// Created by Jamar Jacquelynn on 7/6/21.
//

#include "minishell.h"

void error_handler(t_all *all, int errorcode)
{
	if (errorcode == 1)
	{
		printf("minishell: export: `%s': not a valid identifier\n", all->args[all->arg_pos]);
		return;
	}
}