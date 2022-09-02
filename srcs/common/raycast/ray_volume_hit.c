/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_volume_hit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:39:27 by fmauguin          #+#    #+#             */
/*   Updated: 2022/09/02 16:41:35 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs_utils.h"
#include "utils.h"
#include "minirt.h"
#include "libft.h"
#include <math.h>

t_bool	is_aabb_hit(t_ray ray, t_box aabb)
{
	float	t[6];
	float	tmin;
	float	tmax;

	t[0] = (aabb.min.x - ray.origin.x) / ray.dir.x;
	t[1] = (aabb.max.x - ray.origin.x) / ray.dir.x;
	t[2] = (aabb.min.y - ray.origin.y) / ray.dir.y;
	t[3] = (aabb.max.y - ray.origin.y) / ray.dir.y;
	t[4] = (aabb.min.z - ray.origin.z) / ray.dir.z;
	t[5] = (aabb.max.z - ray.origin.z) / ray.dir.z;
	tmin =  fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])),
			fminf(t[4], t[5]));
	tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])),
			fmaxf(t[4], t[5]));
	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0 && tmin > tmax)
		return (true);
	if (tmin > 0 && tmin > tmax)
		return (true);
	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
		return (false);
	return (true);
}

float	solve_quadratic(float a, float b, float c)
{
	float	discriminant;
	float	t0;
	float	t1;

	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return (-1);
	t0 = (-b - sqrtf(discriminant)) / (2 * a);
	t1 = (-b + sqrtf(discriminant)) / (2 * a);
	if (t0 > t1)
		return (t1);
	return (t0);
}

t_bool	is_sphere_hit(t_ray *ray, t_vol *sp)
{
	t_pos	e;
	t_hit	hit;
	float	abc[3];
	float	t;

	vector_ab(sp->pos, ray->origin, &e);
	abc[0] = dot_product(ray->dir, ray->dir);
	abc[1] = 2.0 * dot_product(e, ray->dir);
	abc[2] = dot_product(e, e) - powf(sp->d / 2, 2);
	t = solve_quadratic(abc[0], abc[1], abc[2]);
	if (t == -1)
		return (false);
	hit.dst_origin = t;
	col_cpy(&sp->col, &hit.col);
	update_hit(&hit);
	return (true);
}

t_bool is_plane_hit(t_ray *ray, t_plane *pl)
{
	float	denom;
	float	t;
	t_pos	e;
	t_hit	hit;

	denom = dot_product(pl->vec3, ray->dir);
	if (fabsf(denom) > 0.0001f) // your favorite epsilon
	{
		vector_ab(ray->origin, pl->pos, &e);
		t = dot_product(e, pl->vec3) / denom;
		if (t >= 0.0001f)
		{
			hit.dst_origin = t;
			col_cpy(&pl->col, &hit.col);
			update_hit(&hit);
			return (true); // you might want to allow an epsilon here too
		}
	}
	return (false);
}

/*t_bool	is_cylinder_hit(t_ray *ray, t_vol *cy)
{
	t_hit	hit;
	float	rabc[4];
	float	t;
	float	d;

	rabc[0] = cy->d / 2;
	rabc[1] = ray->dir.x * ray->dir.x + ray->dir.z * ray->dir.z;
	rabc[2] = 2.0 * (ray->dir.x * (ray->origin.x - cy->pos.x) + ray->dir.z * (ray->origin.z - cy->pos.z));
	rabc[3] = powf((ray->origin.x - cy->pos.x), 2) + powf((ray->origin.z - cy->pos.z), 2) - rabc[0] * rabc[0];
	t = solve_quadratic(rabc[1], rabc[2], rabc[3]);
	if (t == -1)
		return (false);
	d = ray->origin.y + t * ray->dir.y;

	if (d >= cy->pos.y && d <= cy->pos.y + (cy->h * cy->vec3.y))
	{
		hit.dst_origin = rabc[1];
		col_cpy(&cy->col, &hit.col);
		update_hit(&hit);
		return (true);
	}
	return (false);
}*/

t_bool	is_cylinder_hit(t_ray *ray, t_vol *cy)
{
	t_hit	hit;
	t_pos	vec3_cy;
	t_pos	vec3[2];
	float	abcdef[6];
	float	hty[3];

	set_vector(cy->pos.x + cy->h * cy->vec3.x, cy->pos.y + cy->h * cy->vec3.y,
			cy->pos.z + cy->h * cy->vec3.z, &vec3_cy);
	vector_ab(cy->pos, vec3_cy, vec3);
	vector_ab(cy->pos, ray->origin, vec3 + 1);

	abcdef[0] = dot_product(vec3[0], vec3[0]);
	abcdef[1] = dot_product(vec3[0], ray->dir);
	abcdef[2] = dot_product(vec3[0], vec3[1]);

	abcdef[3] = abcdef[0] - abcdef[1] * abcdef[1];
	abcdef[4] = abcdef[0] * dot_product(vec3[1], ray->dir) - abcdef[2] * abcdef[1];
	abcdef[5] = abcdef[0] * dot_product(vec3[1], vec3[1]) - abcdef[2] * abcdef[2]
			- cy->d / 2 * cy->d / 2 * abcdef[0];

	hty[0] = abcdef[4] * abcdef[4] -  abcdef[3] * abcdef[5];
	if (hty[0] < 0.0f)
		return (false);
	hty[0] = sqrtf(hty[0]);
	hty[1] = (-1 * abcdef[4] - hty[0]) / abcdef[3];

	hty[2] = abcdef[2] + hty[1] * abcdef[1];
	if (hty[2] > 0.0 && hty[2] < abcdef[0])
	{
		hit.dst_origin = hty[1];
		col_cpy(&cy->col, &hit.col);
		update_hit(&hit);
		return (true);
	}
	
	hty[1] = (-1 * abcdef[4] + hty[0]) / abcdef[3];

	hty[2] = abcdef[2] + hty[1] * abcdef[1];
	if (hty[2] > 0.0 && hty[2] < abcdef[0])
	{
		hit.dst_origin = hty[1];
		col_cpy(&cy->col, &hit.col);
		update_hit(&hit);
		return (true);
	}
	return (false);
}