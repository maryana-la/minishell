#include "minishell.h"

char	*return_str(int env, char *str)
{
	if (env)
		return (str);
	else
		return (ft_strdup(str));
}

char	*replace_env_with_value(char *str, int i, t_all *all)
{
	int		flag;
	int		env;
	char	*tmp;

	flag = 0;
	env = 0;
	while (str[i] && !(check_set(str[i], "|><")))
	{
		if (str[i] == '\'')
			flag++;
		else if (str[i] == '$' && (flag % 2 == 0))
		{
			env++;
			if (env == 1)
				str = ft_dollar(str, &i, all);
			else
			{
				tmp = str;
				str = ft_dollar(str, &i, all);
				ft_memdel(tmp);
			}
		}
		i++;
	}
	return (return_str(env, str));
}

char	*if_question_mark(char *str, int pos_of_dollar, int *i, char *var)
{
	char	*begin_of_str;
	char	*end_of_line;
	char	*tmp;
	char	*value;
	char	*str1;

	value = ft_itoa(g_status_exit_code);
	begin_of_str = ft_substr(str, 0, pos_of_dollar);
	end_of_line = ft_substr(str, (pos_of_dollar + 2), \
	(ft_strlen(str) - pos_of_dollar - 1));
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

char	*if_numeric(char *str, int pos_of_dollar, int *i, char *var)
{
	char	*begin_of_str;
	char	*end_of_line;
	char	*str1;

	if (ft_strlen(var) == 1)
		end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
	else
		end_of_line = ft_substr(str, pos_of_dollar + 2, \
		(ft_strlen(str) - pos_of_dollar - 1));
	begin_of_str = ft_substr(str, 0, pos_of_dollar);
	str1 = ft_strjoin(begin_of_str, end_of_line);
	ft_memdel(begin_of_str);
	ft_memdel(end_of_line);
	ft_memdel(var);
	return (str1);
}

char	*get_value(t_all *all, char *var)
{
	int		j;
	char	*value;

	j = -1;
	while (all->env_vars[++j].key)
	{
		if (ft_strncmp(all->env_vars[j].key, var, (ft_strlen(var) + 1)) == 0)
			break ;
	}
	if (all->env_vars[j].key)
		value = ft_strdup(all->env_vars[j].value);
	else
		value = ft_strdup("\0");
	ft_memdel(var);
	return (value);
}

char	*place_value(char *str, int pos_of_dollar, int *i, char *value)
{
	char	*begin_of_str;
	char	*end_of_line;
	char	*tmp;
	char	*str1;

	begin_of_str = ft_substr(str, 0, pos_of_dollar);
	end_of_line = ft_substr(str, *i, (ft_strlen(str) - *i + 1));
	tmp = ft_strjoin(begin_of_str, value);
	str1 = ft_strjoin(tmp, end_of_line);
	*i = pos_of_dollar + (int)ft_strlen(value) - 1;
	ft_memdel(tmp);
	ft_memdel(begin_of_str);
	ft_memdel(value);
	ft_memdel(end_of_line);
	return (str1);
}

char	*ft_dollar(char *str, int *i, t_all *all)
{
	char	*var;
	int		pos_of_dollar;
	int		end_of_var;

	pos_of_dollar = *i;
	while (str[++(*i)])
	{
		if (check_set(str[*i], " \t\'\"\\$;|></="))
			break ;
	}
	end_of_var = *i;
	if (end_of_var - pos_of_dollar == 1)
		return (ft_strdup(str));
	var = ft_substr(str, pos_of_dollar + 1, (end_of_var - pos_of_dollar - 1));
	if (!(ft_strncmp(var, "?", 1)))
		return (if_question_mark(str, pos_of_dollar, i, var));
	else if (ft_isdigit(var[0]) != 0)
		return (if_numeric(str, pos_of_dollar, i, var));
	else
		return (place_value(str, pos_of_dollar, i, get_value(all, var)));
}
