
#include "minishell.h"

char	*ft_slash(char *str, int *i)
{
	char *tmp;
	char *tmp2;

	tmp = ft_substr(str, 0, *i);
	tmp2 = ft_strdup(str + *i + 1);
	tmp = ft_strjoin(tmp, tmp2);
	// ++*i;
	*i = *i + 2;
	return(tmp);
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

char *ft_double_quote(char *str, int *i)
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
		// if (str[*i] == '\'')
		// 	str = ft_s_quote(str, i);
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

void ft_parcer(char *str)
{
	int i;

	i = 0;

	// char *line;
	// line = "\0";

	while (str[i])
	{
		if (str[i] == '\'')
		{
			str = ft_s_quote(str, &i);
			printf("tmp'  = %s\n", str);}
		else if (str[i] == '\\'){
		 	str = ft_slash(str, &i);
			printf("tmp\\  = %s\n", str);}
		if (str[i] == '\"'){
			str = ft_double_quote(str, &i);
			printf("tmp\"  = %s\n", str);}
		i++;
	}
	printf("str_e = %s\n", str);
}

int check_inside_s_quote(char *str, int *i)
{
	*i = *i + 1;
	while(str[*i] && str[*i] != '\'')
		*i = *i +1;
	if (str[*i] == '\'')
		return (1);
	return (0);
}

int check_inside_d_quote(char *str, int *i)
{
	*i = *i + 1;
	while(str[*i])
	{
		if (str[*i] == '\"' && str[*i] != '\\')
			break;
		*i = *i + 1;
	}
	if (str[*i] == '\"')
		return (1);
	return (0);
}

int skip_spaces(char *str)
{
	size_t i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	return (i);
}

int check_tokens(char *str, int *i, char token)
{
	int first;
	int diff;

	first = *i;
	*i = *i + 1;
	while (str[*i] == ' ' || str[*i] == '\t')
		*i = *i + 1;
	diff = *i - first;
	if (diff > 1 && str[*i] == token) // if 2 equal tokens with space between
			return (-1);

	if (token == ';')
	{
		if (str[*i] == token || str[*i] == '|' || (str[*i] == '<' && str[*i + 1] == '<' ))
			return (-1);
		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') || str[*i] == '>')
			return (1);
	}
	if (token == '|')
	{
		if (str[*i] == ';' || (str[*i] == '<' && str[*i + 1] == '<' ))
			return (-1);
		if (str[*i] == token || str[*i] == '<'|| (str[*i] == '>' && str[*i + 1] == '>') || str[*i] == '>')
			return (1);
		
	}
	if (token == '>')
	{
		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') || (str[*i] == '|' && diff > 1) || (str[*i] == '>' && str[*i + 1] == '|') || str[*i] == ';')
			return (-1);
		if (str[*i] == '>' || (str[*i] == '|' && diff == 1))
			return (1);
	}
	if (token == '<')
	{
		if (str[*i] == '<' || str[*i] == '|' || str[*i] == ';' || (str[*i] == '>' && diff > 1))
			return (-1);
		if (str[*i] == '>' && diff == 1)
			return (1);
	}
	*i = *i - 1;
	return (1);
}

int ft_preparcer(char *str)
{
	int i;
	// int	s_quote;
	// int d_quote;
	int len;
	int start;

	len = (int)ft_strlen(str) - 1;

	i = skip_spaces(str);
	start = i;
	if (str[i] == ';' || str[i] == '|' || str[len] == '\\' || str[len] == '|' || str[len] == '>' || str[len] == '<')
		return (-1);
	i--;
	while (str[++i])
	{
		if (str[i] == '\'' && (i == start || str[i - 1] != '\\'))
		{
			if (!check_inside_s_quote(str, &i))
				return (-1); // \' is not closed
		}
		else if (str[i] == '\"' && (i == start || str[i - 1] != '\\'))
		{
			if (!check_inside_d_quote(str, &i))
				return (-1); // \" is not closed
		}
		else if (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
			i = i + 2;
		else if (str[i] == ';' || str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (check_tokens(str, &i, str[i]) < 0)
				return (-1); 
		}
	}
	return (1);
}

int env_init(t_all *all, char **env)
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

int main(int argc, char **argv, char **env)
{
	t_all  all;
	int i = 0;
	char **test;

	env_init(&all, env);
	char *str = "echo \"user\" cat >>";

	printf("str_i = %s\n", str);
	if (ft_preparcer(str) > 0)
		ft_parcer(str);
	else
		printf("%s\n", "preparcer error");
	return 0;
}
