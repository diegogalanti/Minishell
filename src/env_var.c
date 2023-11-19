/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tstahlhu <tstahlhu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:05:54 by tstahlhu          #+#    #+#             */
/*   Updated: 2023/11/19 15:05:57 by tstahlhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char   **add_var(t_data *data, char **var, char *new_var)
{
    int	i;
    char    **ret;

	i = 0;
	while (var != NULL && var[i] != NULL)
		i++;
	if (!(ret = safe_malloc(data, ((i + 2) * sizeof(char *)))))
		return (0);
	i = -1;
	while (var != NULL && var[++i] != NULL)
		ret[i] = fs_strdup(data, var[i]);
    ret[i++] = fs_strdup(data, new_var);
	ret[i] = NULL;
    var = free_arr_str(var);
	return (ret);
}

char    **del_var(t_data *data, char **var, char *del_var)
{
    int	i;
    char    **ret;

	i = 0;
	while (var != NULL && var[i] != NULL)
		i++;
	if (!(ret = safe_malloc(data, (i * sizeof(char *)))))
		return (0);
	i = -1;
	while (var != NULL && var[++i] != NULL)
    {
        if (ft_strvcmp(var[i], del_var))
		    ret[i] = fs_strdup(data, var[i]);
    }
	ret[i] = NULL;
    var = free_arr_str(var);
	return (ret);
}
