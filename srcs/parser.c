/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/04 15:29:51 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#define CHARTOF (1.0f / (float)0xff)

void					color_load(t_v4f *target, const unsigned int color)
{
	*target = (t_v4f){
		.x = (float)((color & 0xff000000) >> 24) * CHARTOF,
		.y = (float)((color & 0x00ff0000) >> 16) * CHARTOF,
		.z = (float)((color & 0x0000ff00) >> 8) * CHARTOF,
		.w = (float)((color & 0x000000ff)) * CHARTOF
	};
}

static t_vertex_pack	*parse_setptrs(t_vertex_pack *pack)
{
	const t_obj_stats	*stats = &pack->stats;

	pack->items = ft_memalloc(sizeof(t_vertex_item) * stats->vertex);
	pack->faces = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->fuv = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->uv = ft_memalloc(sizeof(t_v2f) * stats->uv);
	pack->fnormals = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->normals = ft_memalloc(sizeof(t_v3f) * stats->normal);
	if ((!pack->items) || (!pack->faces) || (!pack->fuv) ||
		(!pack->uv) || (!pack->fnormals) || (!pack->normals))
	{
		ft_mfree(6, pack->items, pack->faces, pack->fuv, pack->uv,
				pack->normal, pack->fnormals);
		ft_dprintf(2, "error: failed to malloc !\n");
		return (NULL);
	}
	return (pack);
}

int			parse_obj(t_vertex_pack *pack, const char *filepath)
{
	t_vertex_pack		pack_copy;

	pack->stats = parser_count(filepath);
	if (pack->stats.vertex + pack->stats.faces == 0)
	{
		ft_dprintf(2, "error: no faces or vertex to display\n");
		return (1);
	}
	if (!(parse_setptrs(pack)))
		return (1);
	pack_copy = *pack;
	if ((parse_real(filepath, &pack_copy) < 0) && (ft_mfree(1, pack->items)))
		return (1);
	return (parse_post_process(pack));
}
