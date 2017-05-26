/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_duplicate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 17:39:06 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 17:50:00 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

int						parse_duplicate(t_vertex_pack *pack)
{
	t_v3i			*idx;
	t_vertex_item	*items;
	t_vertex_item	*i;
	int				vidx;
	size_t			p;

	items = malloc(sizeof(t_vertex_item) * (pack->stats.faces * 3));
	if (!(items))
		return (1);
	i = items;
	vidx = 0;
	p = 0;
	while (p < pack->stats.faces)
	{
		idx = &pack->faces[p];
		*(i) = pack->items[idx->x];
		(i++)->uv = pack->items[pack->fuv[p].x].uv;
		*(i) = pack->items[idx->y];
		(i++)->uv = pack->items[pack->fuv[p].y].uv;
		*(i) = pack->items[idx->z];
		(i++)->uv = pack->items[pack->fuv[p].z].uv;
		*idx = (t_v3i){vidx, vidx + 1, vidx + 2};
		vidx += 3;
		p++;
	}
	pack->stats.vertex = pack->stats.faces * 3;
	free(pack->items);
	pack->items = items;
	return (0);
}
