/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmauguin <fmauguin@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 13:04:08 by fmauguin          #+#    #+#             */
/*   Updated: 2022/08/26 17:57:29 by fmauguin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs_utils.h"

//		utils/ft_gnl_rt.c
char	*ft_gnl_rt(int fd);

//		utils/error_msg.c
void	error_msg(char *str);

//		utils/vector_math.c
void	vector_ab(t_pos a, t_pos b, t_pos *vec);
double	dot_product(t_pos a, t_pos b);
double	vector_norm(t_pos vec);
void	vector_add(t_pos a, t_pos b, t_pos *vec);
void	set_vector(float x, float y, float z, t_pos *vec);
//		utils/vector_math2.c
double	dist_ab(const t_pos *a, const t_pos *b);
void	vector_equal(const t_pos src, t_pos  *dest);

//		utils/bbox_utils.c
void	bbox_center(const t_box *box, t_pos *center);
float	bbox_expend(const t_box *box);

//		utils/quicksort_lst_custom.c
void	sort_list_custom(t_list **lst, const t_pos *origin);
int		cmp_nearest_vol(const t_vol *left, const t_vol *right,
			const t_pos *origin);

//		utils/debug.c
void	print_vol(void *vol);
void	print_pos(const t_pos *pos, const char *str);

#endif
