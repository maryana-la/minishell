#include "libft/libft.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>

typedef struct s_env
{
	char		*key;
	char		*value;
	struct s_env *next;
}				t_env;

typedef struct s_all
{
	t_list	*command;
	t_env	*env_list;
}				t_all;
