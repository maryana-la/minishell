#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

//prototypes start


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
	char	cwd[1000];
	char	**envp;
	char	*args[1000];
	int		arg_len;
}				t_all;


//typedef struct		s_all
//{
//	char *command[100];

//	char **envp;
//}					t_all;

void pwd_command (t_all *all);
void env_command (t_all *all);
void export_command(t_all *all);
void env_init(t_all *all, char **env);
void print_env_list(t_all all);
void	add_lst(t_all *all);

//prototypes end

void env_init(t_all *all, char **env)
{
	t_env	*new;
	t_env	*list;
	t_env	*tmp;
	int i = -1;
	int j;
	int shlvl_tmp;
	int fd;

	list = malloc(sizeof(t_env));
	if (!list)
		return;

// init first list
	while(env[0][++i])
	{
		if (env[0][i] == '=')
			break;
	}
	list->key = ft_substr(env[0], 0, i);
	list->value = ft_substr(env[0], i + 1, (ft_strlen(env[0]) - i + 1));
	list->next = NULL;
	all->env_list = list;

// get all the envs
	i = 0;
	while(env[++i])
	{
		j = -1;
		while(env[i][++j] != '\0')
			if (env[i][j] == '=')
				break;
		new = malloc(sizeof(t_env));
		new->key = ft_substr(env[i], 0, j);
		new->value = ft_substr(env[i], j + 1, (ft_strlen(env[i]) - j + 1));
		new->next = NULL;
		list->next = new;
		list = new;
	}

}

void print_env_list(t_all all)
{
	t_env *tmp = all.env_list;
	while (all.env_list && tmp)
	{
		write(1, tmp->key, ft_strlen(tmp->key));
		write(1, "=", 1);
		write(1, tmp->value, ft_strlen(tmp->value));
		write(1, "\n",1);
		tmp = tmp->next;
	}
}



int main(int argc, char **argv, char **envp)
{
	t_all all;
	env_init(&all, envp);

	all.args[0] = "export";
//	all.args[1] = "temp=567";
	all.args[1] = NULL;

	if (!ft_strncmp(all.args[0], "pwd", ft_strlen(all.args[0])))
		pwd_command(&all);
	else if (!ft_strncmp(all.args[0], "env", ft_strlen(all.args[0])))
		env_command(&all);
	else if (!ft_strncmp(all.args[0], "export", ft_strlen(all.args[0])))
		export_command(&all);

}

void pwd_command (t_all *all)
	{
		getcwd(all->cwd, sizeof(all->cwd));
		write(1, all->cwd, ft_strlen(all->cwd));
	}

void env_command (t_all *all)
{
	print_env_list(*all);
}

void export_command(t_all *all)
{
	int i = -1;
	t_env *tmp = all->env_list;
	char *splited[100];

	if (!all->args[1])
	{
		while (tmp)
		{
			write(1, "declare -x ", 11);
			write(1, tmp->key, ft_strlen(tmp->key));
			write(1, "=", 1);
			write(1, tmp->value, ft_strlen(tmp->value));
			write(1, "\n", 1);
			tmp = tmp->next;
		}
		exit(0);
	}
	else
	{
		add_lst(all);
		print_env_list(*all);
	}
}

void	add_lst(t_all *all)
{
	t_env	*list;
	t_env *new = NULL;

	int i = -1;
	while (all->args[1][++i])
	{
		if (all->args[1][i] == '=')
			break;
	}
	new = malloc(sizeof(t_env));
	new->key = ft_substr(all->args[1], 0, i);
	new->value = ft_substr(all->args[1], i + 1, ft_strlen(all->args[1])-i+1);
	new->next = NULL;
	list = all->env_list;
	if (list == NULL)
		all->env_list = new;
	else
	{
		while (all->env_list && list->next)
			list = list->next;
		list->next = new;
	}
}