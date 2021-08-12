#include "minishell.h"

int	check_inside_s_quote(char *str, int *i)
{
	*i = *i + 1;
	while (str[*i] && str[*i] != '\'')
		*i = *i + 1;
	if (str[*i] == '\'')
		return (0);
	return (1);
}

int	check_inside_d_quote(char *str, int *i)
{
	*i = *i + 1;
	while (str[*i])
	{
		if (str[*i] == '\"' && str[*i - 1] != '\\')
			break ;
		*i = *i + 1;
	}
	if (str[*i] == '\"')
		return (0);
	return (1);
}

int	check_tokens_1(char *str, int *i, char token)
{
	int	first;
	int	diff;

	first = *i;
	*i = *i + 1;
	while (str[*i] == ' ' || str[*i] == '\t')
		*i = *i + 1;
	diff = *i - first;
	if (diff > 1 && str[*i] == token)
		return (1);
	return (0);
}

int	check_tokens(char *str, int *i, char token)
{
	if (check_tokens_1(str, i, token) == 1)
		return (1);
	if (token == '|')
	{
		if (str[*i] == ';' || str[*i] == token || (str[*i] == '<' && str[*i \
		+ 1] == '<' ) || str[*i] == '<' || (str[*i] == '>' \
		&& str[*i + 1] == '>') || str[*i] == '>')
			return (1);
	}
	if (token == '>')
	{
		if (str[*i] == '<' || (str[*i] == '>' && str[*i + 1] == '>') \
		|| str[*i] == '|' || str[*i] == ';')
			return (1);
	}
	if (token == '<')
	{
		if (str[*i] == '|' || str[*i] == ';' || (str[*i] == '>'))
			return (1);
	}
	*i = *i - 1;
	return (0);
}

int	preparser_error(t_all *all, char *token, int len)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, token, len);
	write (2, "'\n", 2);
	return (g_status_exit_code = 258);
}

int	ft_preparser_1(char *str, t_all *all, int i)
{
	while (str[++i])
	{
		if (str[i] == '\\' || str[i] == ';')
			return (preparser_error(all, &str[i], 1));
		else if (str[i] == '\'')
		{
			if (check_inside_s_quote(str, &i) == 1)
				return (preparser_error(all, "\'", 1)); // ' is not closed
		}
		else if (str[i] == '\"')
		{
			if (check_inside_d_quote(str, &i) == 1)
				return (preparser_error(all, "\"", 1)); // " is not closed
		}
		else if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (str[i] == '|')
				all->num_of_pipes++;
			if (check_tokens(str, &i, str[i]) == 1)
				return (preparser_error(all, &str[i], 1));
		}
	}
	return (0);
}

int	ft_preparser(char *str, t_all *all)
{
	int	i;
	int	len;

	all->num_of_pipes = 0;
	len = (int)ft_strlen(str) - 1;
	i = 0;
	skip_spaces(str, &i);
	while (str[len] == ' ' || str[len] == '\t')
		len--;
	if (str[i] == '|' || str[len] == '|')
	{
		if (str[i] == str[i + 1] || str[len] == str[len - 1])
			return (preparser_error(all, "||", 2));
		else
			return (preparser_error(all, "|", 1));
	}
	if (str[len] == '<' || str[len] == '>')
		return (preparser_error(all, "newline", 7));
	i--;
	return (ft_preparser_1(str, all, i));
}
