//#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

//prototypes start
typedef struct		s_all
{
	char *command[100];
	char cwd[1000];
	char **envp;
}					t_all;

void pwd_command (t_all *all);
void env_command (t_all *all);
void export_command(t_all *all);

//prototypes end



int main(int argc, char **argv, char **envp)
{
	t_all all;

	all.envp = envp;
	all.command[0] = "export";
	all.command[1] = "temp=567";

	if (!ft_strncmp(all.command[0], "pwd", ft_strlen(all.command[0])))
		pwd_command(&all);
	else if (!ft_strncmp(all.command[0], "env", ft_strlen(all.command[0])))
		env_command(&all);
	else if (!ft_strncmp(all.command[0], "export", ft_strlen(all.command[0])))
		export_command(&all);

}

void pwd_command (t_all *all)
	{
		getcwd(all->cwd, sizeof(all->cwd));
		write(1, all->cwd, ft_strlen(all->cwd));
	}

void env_command (t_all *all)
{
	int i = -1;
	while (all->envp[++i])
	{
		write(1, all->envp[i], ft_strlen(all->envp[i]));
		write(1, "\n", 1);
	}
}

void export_command(t_all *all)
{
	int i = -1;
	char *splited[100];

	if (!all->command[1])
	{
		while (all->envp[++i])
		{
			write(1, "declare -x ", 11);
			write(1, all->envp[i], ft_strlen(all->envp[i]));
			write(1, "\n", 1);
		}
		exit(0);
	}
	else
	{
		int flag = 0;
		int j = 0;
		i = -1;
		while (all->command[1][++i])
		{
			if (all->command[1][i] == '=')
				break;
		}
		splited[0] = ft_substr(all->command[1], 0, i);
		splited[1] = ft_substr(all->command[1], i + 1, ft_strlen(all->command[1])-i+1);

		setenv(splited[0], splited[1], 0);
		pause();
	}
}