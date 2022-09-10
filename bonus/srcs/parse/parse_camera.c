/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 19:19:36 by amahla            #+#    #+#             */
/*   Updated: 2022/09/10 00:45:18 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include "structs_utils.h"
#include "libft.h"
#include "mlx_data.h"
#include "utils.h"
#include <stdio.h>

void	malloc_cam(t_cam **cam, t_scene *scene)
{
	*cam = malloc(sizeof(t_cam));
	if (!*cam)
		exit_parse(scene, "Error\nmalloc fail\n");
}

void	set_camera(t_cam *cam, t_resolut *res)
{
	t_pos	vup;

	set_vector(0, 1, 0, &vup);
	cam->viewport_width = VP_WIDTH * tan((cam->h_fov * (M_PI / 180) / 2));
	cam->viewport_height = cam->viewport_width / res->aspect_ratio;
	vector_equal(cam->vec3, &cam->uvw[2]);
	unit_vector(&cam->uvw[2]);
	cross_product(vup, cam->uvw[2], &cam->uvw[0]);
	cross_product(cam->uvw[2], cam->uvw[0], &cam->uvw[1]);
	vector_equal(cam->uvw[0], &cam->horizontal);
	vector_scale(cam->viewport_width * cam->focal_length, &cam->horizontal);
	vector_equal(cam->uvw[1], &cam->vertical);
	vector_scale(cam->viewport_height * cam->focal_length, &cam->vertical);
	set_vector(cam->pos.x - (cam->horizontal.x / 2) - (cam->vertical.x / 2)
		- (cam->focal_length * cam->uvw[2].x),
		cam->pos.y - (cam->horizontal.y / 2) - (cam->vertical.y / 2)
		- (cam->focal_length * cam->uvw[2].y),
		cam->pos.z - (cam->horizontal.z / 2) - (cam->vertical.z / 2)
		- (cam->focal_length * cam->uvw[2].z), &cam->lower_left_corner);
}

void	camera(t_scene *scene, char *str)
{
	int		i;
	t_cam	*cam;

	i = 1;
	malloc_cam(&cam, scene);
	while (str[i] == ' ')
		i++;
	i += set_pos(scene, &cam->pos, str + i);
	while (str[i] == ' ')
		i++;
	i += set_vec3(scene, &cam->vec3, str + i);
	if (cam->vec3.x == 0 && cam->vec3.y == 0
		&& cam->vec3.z == 0)
		cam->vec3.z = -1.f;
	while (str[i] == ' ')
		i++;
	i += set_h_fov(scene, &cam->h_fov, str + i);
	cam->focal_length = FOCAL_LENGTH;
	unit_vector(&cam->vec3);
	set_camera(cam, scene);
	if (&scene->cam)
		scene->cam = cam;
	ft_lstadd_back(&scene->cameras, ft_lstnew(cam));
}
