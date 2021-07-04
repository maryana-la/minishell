#include "minishell.h"

int check_inside_s_quote(char *str, int *i)
{
	*i = *i + 1;
	while(str[*i] && str[*i] != '\'')
		*i = *i + 1;
	if (str[*i] == '\'')
		return (1);
	return (0);
}

int check_inside_d_quote(char *str, int *i)
{
	*i = *i + 1;
	while(str[*i])
	{
		if (str[*i] == '\\')
			return (0);
		else if (str[*i] == '\"')
			break;
		*i = *i + 1;
	}
	if (str[*i] == '\"')
		return (1);
	return (0);
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

//	if (token == ';')
//	{
//		if (str[*i] == token || str[*i] == '|' || (str[*i] == '<' && str[*i + 1] == '<' ))
//			return (-1);
//		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') || str[*i] == '>')
//			return (1);
//	}
	if (token == '|')
	{
		if (str[*i] == ';' || str[*i] == token || (str[*i] == '<' && str[*i + 1] == '<' ))
			return (-1);
		if (str[*i] == '<'|| (str[*i] == '>' && str[*i + 1] == '>') || str[*i] == '>')
			return (1);
	}
	if (token == '>')
	{
		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') || (str[*i] == '|' && diff > 1) ||
			(str[*i] == '>' && str[*i + 1] == '|') || str[*i] == ';')
			return (-1);
		if (str[*i] == '>' || (str[*i] == '|' && diff == 1))
			return (1);
	}
	if (token == '<')
	{
		if (str[*i] == '|' || str[*i] == ';' || (str[*i] == '>' && diff > 1))
			return (-1);
		if ((str[*i] == '>' && diff == 1) || str[*i] == '<')
			return (1);
	}
	*i = *i - 1;
	return (1);
}

int ft_preparser(char *str)
{
	int i;
	int len;
	int start;

	len = (int)ft_strlen(str) - 1;
	i = 0;
	skip_spaces(str, &i);
	start = i;
	if (str[i] == ';' || str[i] == '|' || str[len] == '\\' || str[len] == '|')
		return (-1);
	i--;
	while (str[++i])
	{
		if (str[i] == '\\' || str[i] == ';')
			return (-1);
		else if (str[i] == '\'')
//			&& (i == start || str[i - 1] != '\\'))
		{
			if (check_inside_s_quote(str, &i) == 0)
				return (-1); // \' is not closed
		}
		else if (str[i] == '\"')
//			&& (i == start || str[i - 1] != '\\'))
		{
			if (check_inside_d_quote(str, &i) == 0)
				return (-1); // \" is not closed
		}
//		else if (str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
//			i = i + 2;
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (check_tokens(str, &i, str[i]) < 0)
				return (-1);
		}
	}
	return (1);
}