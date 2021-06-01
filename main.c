
#include "minishell.h"

char *ft_slash(char *str, int *i)
{
	char *tmp;
	char *tmp2;

	tmp = ft_substr(str, 0, *i);
	tmp2 = ft_strdup(str + *i + 1);
	tmp = ft_strjoin(tmp, tmp2);
	*i = *i + 2;
	return(tmp);
}

int check_set(char c, char *set)
{
	int i;

	i = -1;
	while (set[++i] != '\0')
		if (set[i] == c)
			return (1);
	return 0;
}

void skip_spaces(char *str, int *i)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
}

char *ft_dollar(char *str, int *i, t_all *all)
{
	char	*var;
	int		pos_of_dollar;
	int		end_of_var;
	t_env	*tmp;
	char	*tmp1;
	char	*tmp2;
	char	*value;
/*
** добавить обработку $ $1 $12 $$  $?
*/
	pos_of_dollar = *i;
	while(str[++*i]) //find end of variable
		if(check_set(str[*i], " \t\'\"\\$;|><"))
			break;
	end_of_var = *i;

	if (end_of_var - pos_of_dollar == 1)
		return(str);

//cut variable
	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar -1));
	// printf("var = %s\n", var);

	if (ft_isdigit(var[0]) != 0) // check all ascii symbols
	{
		if(ft_strlen(var) == 1)
		{	
			tmp1 = ft_substr(str, 0, pos_of_dollar);
			tmp2 = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
			tmp1 = ft_strjoin(tmp1, tmp2);
			return (tmp1);
		}
		else
		{
			tmp1 = ft_substr(str, 0, pos_of_dollar);
			tmp2 = ft_substr(str, pos_of_dollar + 2, (ft_strlen(str) - pos_of_dollar - 1));
			tmp1 = ft_strjoin(tmp1, tmp2);
			return (tmp1);
		}
	}
//find variable in the lists
	tmp = all->env_list;
	while (all->env_list && tmp->next != NULL)
	{
		if (!ft_strncmp(tmp->key, var, (ft_strlen(var) + 1)))
			break;
		tmp = tmp->next;
	}
	if (ft_strncmp(tmp->key, var, (ft_strlen(var) + 1)))//если долистал до конца, но переменная не найдена
		value = "\0";	
	else
		value = ft_strdup(tmp->value);
	tmp1 = ft_substr(str, 0, pos_of_dollar); // cut till $
	tmp2 = ft_substr(str, *i, (ft_strlen(str) - *i + 1)); // cut after variable
	tmp1 = ft_strjoin(tmp1, value);
	tmp1 = ft_strjoin(tmp1, tmp2);
	*i = pos_of_dollar + ft_strlen(value) - 1;
	return (tmp1);
}

void ft_s_quote(char *str, char **arg, int *i, int *j_tmp)
{
	// int j;
	// char *tmp;
	// char *tmp1;
	// char *tmp2;

	// j = *i;
	while (str[++*i])
	{	if (str[*i] == '\'')
			break;
		else
		{
			*arg[*j_tmp] = str[*i];
			(*j_tmp)++;
		}
	}
	// tmp = ft_substr(str, 0, j); // printf("tmp = %s\n", tmp); - before '
	// tmp1 = ft_substr(str, j + 1, *i - j - 1); // printf("tmp1 = %s\n", tmp1); - cut inside ' ' 
	// tmp2 = ft_strdup(str + *i + 1); // ft_strdup(&str[i+1]) - cut after ' ' 
	// tmp2 = ft_substr(str, i + 1, (ft_strlen(str) - i - 1)); // printf("tmp2 = %s\n", tmp2);
	// tmp1 = ft_strjoin(tmp1, tmp2); // printf("tmp = %s\n", tmp);
	// *i = *i - 2;
	// *j_tmp += ft_strlen(tmp1); 
	// return (tmp1);
}

char *ft_double_quote(char *str, int *i, int *j_tmp)
{
	int j;
	char *tmp;
	char *tmp1;
	char *tmp2;

	j = *i;

	while (str[++(*i)])
	{
		if (str[*i] == '\\' && (str[*i + 1] == '$' || str[*i + 1] == '\'' || str[*i + 1] == '\"' || str[*i + 1] == '\\'))
			str = ft_slash(str, i);
		// if (str[*i] == '$')
		// 	str = ft_dollar(str, i, all);
		if (str[*i] == '\"')
			break ;
	}
	tmp1 = ft_substr(str, j + 1, *i - j - 1); // printf("tmp1 = %s\n", tmp1); - cut inside " " 
	*j_tmp += ft_strlen(tmp1);
	return (tmp1);
}

char	*replace_env_with_value(char *str, t_all *all)
{
	int flag;
	int i;

	flag = 0;
	i = -1;
// заменяем переменные только до следующей команды
	while(str[++i] && !(check_set(str[i], "|;><")))
	{
		if(str[i] == '\'')
		{
			if(flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		// else if (str[i] == '\\' && flag == 0)
		//  	str = ft_slash(str, &i);
		else if(str[i] == '$' && str[i - 1] != '\\' && flag == 0)
			str = ft_dollar(str, &i, all);
	}
	return(str);
}

int get_arg_len(char *str, int i)
{
	int len;
	int pos;

	pos = i;
	len = 0;
	while(str[i] && !check_set(str[i], " \t|;<>"))
	{
		if (str[i] == '\'')
		{
			i++;
		    while (str[i] != '\'')
            {
				len++;
                i++;
            }
			i++;
		}
        else if (str[i] == '\"')
        {
            i++;
            while (str[i] != '\"')
            {
                len++;
                i++;
            }
            i++;
        }
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

char *check_lower_case(char *str)
{
	int i;
	char *dest;

	i = 0;
	dest = malloc(sizeof(char) * ft_strlen(str));
	while (str[i] != 0)
	{
		dest[i] = ft_tolower(str[i]);
		i++;
	}
	if (!ft_strncmp(dest, "echo", 5) || !ft_strncmp(dest, "pwd", 4) || !ft_strncmp(dest, "env", 4))
	{
		free(str);
		return(dest);
	}
	free(dest);
	return(str);
}

void ft_put_str_to_struct(char *arg, t_all *all)
{
	int i;
	char **tmp;

	i = 0;

	if (all->args == 0)
    {
		arg = check_lower_case(arg);
		all->args = malloc(sizeof(char *) * 1);
        all->args[0] = arg;
        all->args[1] = 0;
        i = -1;
        // while (all->args[++i] != 0)
        //     printf("%s\n", all->args[i]);
    }
	else if (all->args)
	{
		while (all->args[i] != 0)
			i++;
		tmp = malloc(sizeof(char *) * (i + 1));
		i = 0;
		while (all->args[i] != 0)
		{
			tmp[i] = ft_strdup(all->args[i]);
			i++;
		}
		tmp[i] = arg;
		tmp[++i] = 0;

		i = 0;
		free(all->args);
		all->args = tmp;
		i = -1;
		while (all->args[++i] != 0)
			printf("%s\n", all->args[i]);
	}
}

void ft_parcer(char *str, t_all *all)
{
	int i;
	int j;
	int len;
	char *tmp;
	char *from_quote;

	str = replace_env_with_value(str, all); // заменяем в строке переменные окружения
	printf("%s\n", str);
	i = 0;

	while(str[i] && !(check_set(str[i], "|;><")))
	{
		skip_spaces(str, &i);
		len = get_arg_len(str, i);
		printf("%d\n", len);
		tmp = malloc(sizeof(char) * (len + 1));
//		ft_bzero(tmp, len);
		j = 0;
		while (str[i] && !check_set(str[i], " \t|;<>"))
		{
			if (str[i] == '\'')
			{
				while (str[++i] && str[i] != '\'')
				{
					tmp[j] = str[i];
					j++;
				}
				j--;
			}
			else if (str[i] == '\"')
            {
                while (str[++i] && str[i] != '\"')
                {
                    if (str[i] == '\\' && (str[i + 1] == '$' || str[i + 1] == '\'' || str[i + 1] == '\"' || str[i + 1] == '\\'))
                        tmp[j] = str[++i];
                    else
                        tmp[j] = str[i];
                    j++;
                }
                j--;
            }
			else if (str[i] == '\\')
				tmp[j] = str[++i];
			else
				tmp[j] = str[i];
			i++;
			j++;
		}
		tmp[j] = '\0';
		ft_put_str_to_struct(tmp, all);
		// printf("%s\n", tmp);
		i++;
	}
}

int env_init(t_all *all, char **env) // env init with lists:
{
	t_env	*new;
	t_env	*list;
	t_env	*tmp;
	int i = -1;
	int j;
	int shlvl_tmp;
	int fd;

	fd = open("proba.txt", O_RDWR);


	list = malloc(sizeof(t_env));
	if (!list)
		return(0);

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
	while(env && env[++i])
	{
		j = -1;
		while(env[i][++j] != '\0')
			if (env[i][j] == '=')
				break;
		new = malloc(sizeof(t_env));
		if (!new)
			return (0);
		new->key = ft_substr(env[i], 0, j);
		new->value = ft_substr(env[i], j + 1, (ft_strlen(env[i]) - j + 1));
		new->next = NULL;
		list->next = new;
		list = new;
	}

// to inc SHLVL
	shlvl_tmp = 0;
	tmp = all->env_list;
	while (all->env_list && tmp) //find variable in the lists
	{
		if (ft_strncmp(tmp->key, "SHLVL", 6) == 0)
		{
			shlvl_tmp = ft_atoi(tmp->value) + 1;
			tmp->value = ft_itoa(shlvl_tmp);
			break;
		}
		tmp = tmp->next;
		if(!tmp->next) // if no SHVLV - set it to 1
		{
			new = malloc(sizeof(t_env));
			new->key = ft_strdup("SHLVL");
			new->value = ft_strdup("1");
			new->next = NULL;
			tmp->next = new;
			break;
		}
	}

/* just to print */
	tmp = all->env_list; 
	while (all->env_list && tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

void init_all(t_all *all)
{
	all->args = 0;
}

int main(int argc, char **argv, char **env)
{
	t_all  all;
	int i = 0;
	char **test;

	init_all(&all);
	env_init(&all, env);
	char *str = "ECHO $SHLVL'pwd $PATH' \"$PAGER$LSCOLORS\"$;l$XPC_FLAGS\'ffrsvdd\'";

	printf("str_i = %s\n", str);
	if (ft_preparcer(str) > 0)
		ft_parcer(str, &all);
	else
		printf("%s\n", "preparcer error");
	return 0;
}
