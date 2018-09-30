/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 15:21:46 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/30 18:27:33 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <math.h>

static void				parse_compute_tangeants(struct s_object *object)
{
	size_t				p;
	t_v3f				tangeant;

	p = object->stats.vertex;
	while (p--)
	{
		if (object->items[p].normal.z >= 1.0f)
			tangeant = (t_v3f){0.0f, 1.0f, 0.0f};
		else
			tangeant = (t_v3f){0.0f, 0.0f, 1.0f};
		tangeant = geo_cross(tangeant, object->items[p].normal);
		object->items[p].tangeant = tangeant;
	}
}

int						parse_post_process(struct s_object *object)
{
	const int			mv = (int)object->stats.vertex - 1;
	size_t				p;

	p = object->stats.faces;
	while (p--)
	{
		if ((object->faces[p].x > mv) || (object->faces[p].y > mv) ||
				(object->faces[p].z > mv))
		{
			ft_printf("%lu -> %d %d %d\n", p, object->faces[p].x,
					object->faces[p].y, object->faces[p].z);
			ft_dprintf(2, "error: invalid face in obj file !\n");
			return (1);
		}
	}
	fixcenter(object);
	parse_duplicate(object);
	parse_compute_tangeants(object);
	return (0);
}
