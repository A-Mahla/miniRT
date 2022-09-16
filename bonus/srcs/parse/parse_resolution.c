/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_resolution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 18:06:57 by amahla            #+#    #+#             */
/*   Updated: 2022/09/16 16:58:39 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs_utils.h"
#include "minirt.h"
#include "libft.h"

void	resolution_aliasing(char *str, int *i, t_scene *scene)
{
	const char	*al = "antialiasing:";
	int			start;

	scene->resolut.aliasing = 1;
	if (str[*i] == '\n')
		return ;
	while (str[*i] == ' ')
		(*i)++;
	start = *i;
	while (str[*i])
	{
		if (str[*i - 1] == ':' && !ft_strncmp(str + start, al, *i - start))
		{
			if (!ft_isdigit(str[*i])
				|| ft_atoi_err(str + *i, &scene->resolut.aliasing)
				|| scene->resolut.aliasing < 1 || scene->resolut.aliasing > 10)
				exit_parse(scene, NULL);
			while (ft_isdigit(str[*i]))
				(*i)++;
			return ;
		}
		(*i)++;
	}
	exit_parse(scene, NULL);
}

void	resolution(t_scene *scene, char *str)
{
	int	i;

	i = 1;
	while (str[i] == ' ')
		i++;
	if (!ft_isdigit(str[i]) || ft_atoi_err(str + i, &scene->resolut.win_width)
		|| scene->resolut.win_width < 1 || scene->resolut.win_width > 1920)
		exit_parse(scene, NULL);
	while (ft_isdigit(str[i]))
		i++;
	if (str[i] != ' ')
		exit_parse(scene, NULL);
	while (str[i] == ' ')
		i++;
	if (!ft_isdigit(str[i]) || ft_atoi_err(str + i, &scene->resolut.win_height)
		|| scene->resolut.win_height < 1 || scene->resolut.win_height > 1080)
		exit_parse(scene, NULL);
	while (ft_isdigit(str[i]))
		i++;
	resolution_aliasing(str, &i, scene);
	if (str[i] != '\n')
		exit_parse(scene, NULL);
	scene->resolut.aspect_ratio = scene->resolut.win_width;
	scene->resolut.aspect_ratio /= scene->resolut.win_height;
	scene->resolut.is_set = true;
}
