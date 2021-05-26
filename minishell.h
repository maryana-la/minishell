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
	t_env	*env_list;
	t_env	*env_secret;
	char	**envp;
	char	**args;
	int		arg_len;
}				t_all;
