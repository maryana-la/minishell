#include "../libft/libft.h"
#include <stdio.h>

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
	while(str && str[*i] != '\'')
	{
		if (str[*i] == ';' || str[*i] == '|')
			return (0);
		*i++;
	}
	if (str[*i] == '\'')
		return (1);
	return (0);
}

int check_inside_d_quote(char *str, int *i)
{

}

int ft_preparcer(char *str)
{
	int i;
	int	s_quote;
	int d_quote;
	size_t len;

	len = ft_strlen(str);

	i = -1;
	s_quote = 0;
	d_quote = 0;
	if (str[0] == ';' || str[0] == '|' || str[len - 1] == '\\')
		return (0);
	while (str[++i])
	{
		if (str[i] == '\'')
			if (!check_inside_s_quote)
				return (-1); // \' is not closed
		else if (str[i] == '\"')
			if (!check_inside_d_quote)
				return (-1); // \" is not closed
		else if (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
			i = i + 2;
	}

	if ((s_quote % 2) || (d_quote % 2))
		return (0);
	return (1);
}


int main(int argc, char **argv, char **env)
{
	
	char *str = "'comma\\'nndsdsn''che\"ck'_r\"___\"est'_ofthe_l\"ine";
	printf("str_i = %s\n", str);
	// if (ft_preparcer(str))
		ft_parcer(str);
	// else
	// 	printf("%s\n", "wrong amount of quotes");
	return 0;
}
