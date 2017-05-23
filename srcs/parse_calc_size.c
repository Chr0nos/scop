/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_calc_size.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/02 19:13:38 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/23 18:31:33 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

/*
** return the full data size:
** - vertexes
** - faces
** - normals items
** - uv indexes
** as a size_t
*/

size_t			parse_calc_size(const t_obj_stats *stats)
{
	size_t		size;

	size = sizeof(t_vertex_pack) +
		(sizeof(t_v3f) * stats->vertex) +
		(sizeof(t_v2f) * stats->uv) +
		(sizeof(unsigned char) * stats->faces) +
		(sizeof(t_v3i) * stats->faces * 2) +
		(sizeof(t_v3f) * stats->normal) +
		(sizeof(t_vertex_item) * stats->vertex);
	size += 8;
	return (size);
}
