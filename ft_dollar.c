#include "minishell.h"

char	*replace_env_with_value(char *str, int i, t_all *all)
{
	int flag;
	int env;
	char *tmp;

	flag = 0;
	env = 0;
	tmp = NULL;
	// заменяем переменные только до следующей команды
	while(str[i] && !(check_set(str[i], "|><")))
	{
		if(str[i] == '\'')
		{
			if(flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		else if(str[i] == '$' && flag == 0)
		{
			env++;
			if (env == 1)
				str = ft_dollar(str, &i, all);
			else //to avoid leaks if not first $
			{
				tmp = str;
				str = ft_dollar(str, &i, all);
				ft_memdel(tmp);
			}
		}
		i++;
	}
	if (env)
		return(str);
	else
		return(ft_strdup(str));
}

char	*ft_dollar(char *str, int *i, t_all *all) //done leaks
{
	char	*var;
	int		pos_of_dollar;
	int		end_of_var;
	char	*begin_of_str;
	char	*end_of_line;
	char	*tmp;
	char	*value;
	char	*str1;

	pos_of_dollar = *i;
	while(str[++(*i)]) //find end of variable
	{
		if(check_set(str[*i], " \t\'\"\\$;|></="))
			break;
	}
	end_of_var = *i;

	if (end_of_var - pos_of_dollar == 1)
		return(ft_strdup(str));
		//cut variable
	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar -1));

	if (!(ft_strncmp(var, "?", 1)))
	{
		value = ft_itoa(g_status_exit_code);
		begin_of_str = ft_substr(str, 0, pos_of_dollar);
		end_of_line = ft_substr(str, (pos_of_dollar + 2), (ft_strlen(str) - pos_of_dollar - 1));
		tmp = ft_strjoin(begin_of_str, value);
		str1 = ft_strjoin(tmp, end_of_line);
		*i = pos_of_dollar + (int)ft_strlen(value) - 1;
		ft_memdel(tmp);
		ft_memdel(begin_of_str);
		ft_memdel(value);
		ft_memdel(end_of_line);
		ft_memdel(var);
		return(str1);
	}
	else if (ft_isdigit(var[0]) != 0) // if numeric
	{
		if(ft_strlen(var) == 1)
			end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
		else
			end_of_line = ft_substr(str, pos_of_dollar + 2, (ft_strlen(str) - pos_of_dollar - 1));
		begin_of_str = ft_substr(str, 0, pos_of_dollar);
		str1 = ft_strjoin(begin_of_str, end_of_line);
		ft_memdel(begin_of_str);
		ft_memdel(end_of_line);
		ft_memdel(var);
		return (str1);
	}
//find variable in the lists
	int j = -1;
	while (all->env_vars[++j].key) //find variable in the lists
	{
		if (ft_strncmp(all->env_vars[j].key, var, (ft_strlen(var) + 1)) == 0)
			break;
	}
	if (all->env_vars[j].key) //если долистал до конца или вылетел из цикла
		value = ft_strdup(all->env_vars[j].value);
	else
		value = ft_strdup("\0");
	begin_of_str = ft_substr(str, 0, pos_of_dollar); // cut till $
	end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1)); // cut after variable
	tmp = ft_strjoin(begin_of_str, value);
	str1 = ft_strjoin(tmp, end_of_line);
	*i = pos_of_dollar + (int)ft_strlen(value) - 1;
	ft_memdel(tmp);
	ft_memdel(begin_of_str);
	ft_memdel(value);
	ft_memdel(end_of_line);
	ft_memdel(var);
	return (str1);
}
