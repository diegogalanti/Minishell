/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:05:54 by tstahlhu          #+#    #+#             */
/*   Updated: 2024/01/21 17:14:24 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*char   **add_var(t_data *data, char **var, char *new_var)
{
	int	i;
	char    **ret;

	i = 0;
	while (var != NULL && var[i] != NULL)
		i++;
	if (!(ret = safe_malloc(data, ((i + 2) * sizeof(char *)))))
		return (NULL);
	i = -1;
	while (var != NULL && var[++i] != NULL)
		ret[i] = fs_strdup(data, var[i]);
	ret[i++] = fs_strdup(data, new_var);
	ret[i] = NULL;
	return (ret);
}*/

/* del_var: deletes given variable (del_var) 
            from environment varialbe list (data->env)
	This is done by allocating an array one size smaller than */

char	**del_var(t_data *data, char **var, char *del_var)
{
	int		i;
	int		j;
	char	**ret;

	i = 0;
	j = 0;
	if (!is_var(var, del_var))
		return (var);
	while (var != NULL && var[i] != NULL)
		i++;
	ret = safe_malloc(data, (i * sizeof(char *)));
	if (!ret)
		return (NULL);
	i = -1;
	while (var != NULL && var[++i] != NULL)
	{
		if (ft_strvcmp(var[i], del_var))
			ret[i - j] = fs_strdup(data, var[i]);
		else
			j++;
	}
	ret[i + j] = NULL;
	return (ret);
}

char	**add_mod_var(t_data *data, char **var, char *new_var)
{
	int		i;
	int		add;
	char	**ret;

	add = 1 - is_var(var, new_var);
	i = 0;
	while (var != NULL && var[i] != NULL)
		i++;
	ret = safe_malloc(data, ((i + 1 + add) * sizeof(char *)));
	if (!ret)
		return (NULL);
	i = -1;
	while (var != NULL && var[++i] != NULL)
	{
		if (ft_strvcmp(var[i], new_var))
			ret[i] = fs_strdup(data, var[i]);
		else
			ret[i] = fs_strdup(data, new_var);
	}
	if (add == 1)
		ret[i] = fs_strdup(data, new_var);
	ret[++i] = NULL;
	return (ret);
}

/* find_var: iterates through environment variable list 
            and checks if var_name is a variable
	In case it is, it returns a pointer to the value of the variable.
	In case it is not, it returns a NULL pointer.*/

char	*find_var(char **var, char *var_name)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	if (!var_name || !var)
		return (NULL);
	while (var != NULL && var[++i] != NULL)
	{
		if (!ft_strvcmp(var[i], var_name))
		{
			while (var[i][j] != '=')
				j++;
			return (var[i] + (j + 1));
		}
	}
	return (NULL);
}

/* is_var: iterates through environment variable list 
        and checks if s is a variable
	In case it is, it returns 1.
	In case it is not, it returns 0.*/

int	is_var(char **var, char *s)
{
	int	i;

	i = -1;
	if (!s)
		return (0);
	while (var != NULL && var[++i] != NULL)
	{
		if (!ft_strvcmp(var[i], s))
			return (1);
	}
	return (0);
}
