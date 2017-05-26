/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_duplicate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 17:39:06 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 20:38:32 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

/*
** here the idea is to set each x, y and z (assigned to a face)
** to do so we use offset, 0 = x, 1 = y, 2 = z
** in the first time we copy the full vertex atribute
** then we override uv and normal using fuv and fnormals
*/

static void				load_params(t_vertex_pack *pack, t_vertex_item *item,
		const size_t p, size_t offset)
{
	const int		index_face = ((int*)&(pack->faces[p]))[offset];
	const int		index_uv = ((int*)&(pack->fuv[p]))[offset];
	const int		index_nm = ((int*)&(pack->fnormals[p]))[offset];

	(void)offset;
	item->position = pack->items[index_face].position;
	item->color = pack->items[index_face].color;
	item->uv = pack->uv[index_uv];
	item->normal = pack->normals[index_nm];
}

int						parse_duplicate(t_vertex_pack *pack)
{
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
		load_params(pack, i++, p, 0);
		load_params(pack, i++, p, 1);
		load_params(pack, i++, p, 2);
/*
		(void)load_params;
		*i = pack->items[pack->faces[p].x];
		i->uv = pack->uv[pack->fuv[p].x];
		i->normal = pack->normals[pack->fnormals[p].x];
		i++;

		*i = pack->items[pack->faces[p].y];
		i->uv = pack->uv[pack->fuv[p].y];
		i->normal = pack->normals[pack->fnormals[p].y];
		i++;

 		*i = pack->items[pack->faces[p].z];
		i->uv = pack->uv[pack->fuv[p].z];
		i->normal = pack->normals[pack->fnormals[p].z];
		i++;
*/
		pack->faces[p] = (t_v3i){vidx, vidx + 1, vidx + 2};
		vidx += 3;
		p++;
	}
	pack->stats.vertex = pack->stats.faces * 3;
	ft_mfree(5, pack->items, pack->uv, pack->fuv, pack->normals,
		pack->fnormals);
	pack->items = items;
	return (0);
}
