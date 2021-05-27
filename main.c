
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

	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar -1)); //cut variable
	printf("var = %s\n", var);
	
	tmp = all->env_list;
	while (all->env_list && tmp->next != NULL) //find variable in the lists
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

char *ft_s_quote(char *str, int *i)
{
	int j;
	char *tmp;
	char *tmp1;
	char *tmp2;

	j = *i;
	while (str[++*i])
	{	if (str[*i] == '\'')
			break;
	}
	tmp = ft_substr(str, 0, j); // printf("tmp = %s\n", tmp);
	tmp1 = ft_substr(str, j + 1, *i - j - 1); // printf("tmp1 = %s\n", tmp1);
	tmp2 = ft_strdup(str + *i + 1); // ft_strdup(&str[i+1]) // tmp2 = ft_substr(str, i + 1, (ft_strlen(str) - i - 1)); // printf("tmp2 = %s\n", tmp2);
	tmp = ft_strjoin(tmp, tmp1); // printf("tmp = %s\n", tmp);
	tmp = ft_strjoin(tmp, tmp2); // printf("tmp2 = %s\n", tmp);
	*i = *i - 2;
	return (tmp);
}

char *ft_double_quote(char *str, int *i, t_all *all)
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
	tmp = ft_substr(str, 0, j); // printf("tmp = %s\n", tmp);
	tmp1 = ft_substr(str, j + 1, *i - j - 1); // printf("tmp1 = %s\n", tmp1);
	tmp2 = ft_strdup(str + *i + 1); // ft_strdup(&str[i+1]) // tmp2 = ft_substr(str, i + 1, (ft_strlen(str) - i - 1)); // printf("tmp2 = %s\n", tmp2);
	tmp = ft_strjoin(tmp, tmp1); // printf("tmp = %s\n", tmp);
	tmp = ft_strjoin(tmp, tmp2); // printf("tmp2 = %s\n", tmp);
	return (tmp);
}

int	find_enf_of_arg(char *str, int i, t_all *all)
{
	int begin;
	int len;

	begin = i;
	len = 0;
	while (check_set(str[i], " \t|;<>") == 0)
	{
		if (str[i] == '\'')
			str = ft_s_quote(str, &i);
		else if (str[i] == '\\')
		 	str = ft_slash(str, &i);
		else if (str[i] == '\"')
			str = ft_double_quote(str, &i, all);
		else if (str[i] == '$')
			str = ft_dollar(str, &i, all);
		else
			i++;
		// if (str[i] == '\'')
		// 	while (str[++i] != '\'')
		// 		len++;
		// else if (str[i] == '\"')
		// {

		// }
		// else if (str[i] == '$')
		// {

		// }
		// i++;
		// len++;
	}
	len = i - begin;
	return(len);
}

char	*replace_env_with_value(char *str, t_all *all)
{
	int flag;
	int i;

	flag = 0;
	i = 0;
	while(str[++i] && !(check_set(str[i], "|;><")))
	{
		if(str[i] == '\'')
		{
			if(flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		else if(str[i] == '$' && (flag == 0))
			str = ft_dollar(str, &i, all);
	}
	return(str);
}

void ft_parcer(char *str, t_all *all)
{
	int i;
	
	str = replace_env_with_value(str, all); // заменяем в строке переменные окружения
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			str = ft_s_quote(str, &i);
		else if (str[i] == '\\')
		 	str = ft_slash(str, &i);
		else if (str[i] == '\"')
			str = ft_double_quote(str, &i, all);
		// else if (str[i] == '$')
		// 	str = ft_dollar(str, &i, all);
		i++;
	}
	printf("str_e = %s\n", str);
}

int env_init(t_all *all, char **env) // env init with lists:
{
	t_env	*new;
	t_env	*list;
	int i = -1;
	int j;

	list = malloc(sizeof(t_env));
	if (!list)
		return(0);
	while(env[0][++i])
	{
		if (env[0][i] == '=')
			break;
	}
	list->key = ft_substr(env[0], 0, i);
	list->value = ft_substr(env[0], i + 1, (ft_strlen(env[0]) - i + 1));
	list->next = NULL;
	all->env_list = list;

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
		// printf("%s = %s\n", list->key, list->value);
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
	char *str = "echo $USER'pwd$PATH'$PAGER$LSCOLORSq$;l$XPC_FLAGSs";

	printf("str_i = %s\n", str);
	if (ft_preparcer(str) > 0)
		ft_parcer(str, &all);
	else
		printf("%s\n", "preparcer error");
	return 0;
}
