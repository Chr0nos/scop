/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 15:21:46 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:16:00 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

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
	return (0);
}
