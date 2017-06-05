/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 15:21:46 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/05 19:16:09 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static inline void		parse_tbn_compute(t_vertex_item *vertex,
		const t_v2f *delta_uv, const t_v3f *edge)
{
	float		f;
	t_v3f		t;
	t_v3f		b;

	f = 1.0f / (delta_uv[0].x * delta_uv[1].y - delta_uv[1].x * delta_uv[0].y);
	t.x = f * (delta_uv[1].y * edge[0].x - delta_uv[0].y * edge[1].x);
	t.y = f * (delta_uv[1].y * edge[0].y - delta_uv[0].y * edge[1].y);
	t.z = f * (delta_uv[1].y * edge[0].z - delta_uv[0].y * edge[1].z);
	b.x = f * (-delta_uv[1].x * edge[0].x + delta_uv[0].x * edge[1].x);
	b.y = f * (-delta_uv[1].x * edge[0].y + delta_uv[0].x * edge[1].y);
	b.z = f * (-delta_uv[1].x * edge[0].z + delta_uv[0].x * edge[1].z);
	vertex->tangent = geo_normv(t);
	vertex->bitangent = geo_normv(b);
	vertex->normal = geo_normv(vertex->normal);
}

static void				parse_tbn(t_vertex_pack *pack)
{
	size_t				p;
	t_vertex_item		*vertex[3];
	t_v2f				delta_uv[2];
	t_v3f				edge[2];

	p = pack->stats.faces;
	while (p--)
	{
		vertex[0] = &pack->items[pack->faces[p].x];
		vertex[1] = &pack->items[pack->faces[p].y];
		vertex[2] = &pack->items[pack->faces[p].z];
		edge[0] = geo_subv3(vertex[1]->position, vertex[0]->position);
		edge[1] = geo_subv3(vertex[2]->position, vertex[0]->position);
		delta_uv[0] = geo_subv2f(vertex[1]->uv, vertex[0]->uv);
		delta_uv[1] = geo_subv2f(vertex[2]->uv, vertex[0]->uv);
		parse_tbn_compute(vertex[0], delta_uv, edge);
		vertex[1]->tangent = vertex[0]->tangent;
		vertex[2]->tangent = vertex[0]->tangent;
	}
}

int						parse_post_process(t_vertex_pack *pack)
{
	const int			mv = (int)pack->stats.vertex - 1;
	size_t				p;

	p = pack->stats.faces;
	while (p--)
	{
		if ((pack->faces[p].x > mv) || (pack->faces[p].y > mv) ||
				(pack->faces[p].z > mv))
		{
			ft_printf("%lu -> %d %d %d\n", p, pack->faces[p].x,
					pack->faces[p].y, pack->faces[p].z);
			ft_dprintf(2, "error: invalid face in obj file !\n");
			return (1);
		}
	}
	fixcenter(pack);
	parse_duplicate(pack);
	parse_tbn(pack);
	return (0);
}
