#include "minishell.h"

void	error_handler(char *arg, int errorcode)
{
	if (errorcode == 1)
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return ;
	}
	if (errorcode == 2)
	{
		write(2, "minishell: unset: `", 19);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return ;
	}
}

void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void	ft_memdel_double(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}

void	exec_error_print(char *cmnd, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmnd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void	ft_free_env(t_env *env)
{
	int	i;

	i = -1;
	while (env[++i].key)
	{
		ft_memdel(env[i].key);
		ft_memdel(env[i].value);
	}
	ft_memdel(env);
}
