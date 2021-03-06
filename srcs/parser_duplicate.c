/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_duplicate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 17:39:06 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:16:51 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

/*
** load all sub data into the t_vertex_item structure for the current item
** current item is determined by "p", offset is used to know wich composant we
** are setting (x, y or z) into faces, uv, and normals
** the function also take care about vertex painting: it's create the colors
** for the faces detection (key C)
*/

static void				load_params(struct s_object *object, t_vertex_item *item,
		const size_t p, size_t offset)
{
	const int		index_face = ((int*)&(object->faces[p]))[offset];
	const int		index_uv = ((int*)&(object->fuv[p]))[offset];
	const int		index_nm = ((int*)&(object->fnormals[p]))[offset];
	static int		c = 0;

	item->position = object->items[index_face].position;
	if (c > 2)
		c = 0;
	color_load(&item->color,
			(unsigned int[3]){0xff000000, 0x00ff0000, 0x0000ff00}[c++]);
	if ((index_uv >= 0) && (index_uv < (int)object->stats.uv))
		item->uv = object->uv[index_uv];
	if ((index_nm >= 0) && (index_nm < (int)object->stats.normal))
		item->normal = object->normals[index_nm];
}

/*
** allocate the good size of memory based on the number of faces
** in case of error the functionr display an error message on stderr then
** return NULL
*/

static t_vertex_item	*parse_duplicate_alloc(size_t faces)
{
	const size_t		size = sizeof(t_vertex_item) * (faces * 3);
	t_vertex_item		*items;

	ft_printf("allocating %lu bytes of memory for faces\n", size);
	items = ft_memalloc(size);
	if (!items)
		ft_dprintf(2, "error: failed to malloc ! we are doomed !\n");
	return (items);
}

/*
** duplicate all vertex for each faces, it's prevents bugs with uv mapping
** because two faces can share the sames vertexes, in this case the vertex must
** be duplicated, take more memory but less cpu
*/

int						parse_duplicate(struct s_object *object)
{
	t_vertex_item	*items;
	t_vertex_item	*i;
	int				vidx;
	size_t			p;

	items = parse_duplicate_alloc(object->stats.faces);
	if (!(items))
		return (1);
	i = items;
	vidx = 0;
	p = 0;
	while (p < object->stats.faces)
	{
		load_params(object, i++, p, 0);
		load_params(object, i++, p, 1);
		load_params(object, i++, p, 2);
		object->faces[p] = (t_v3i){vidx, vidx + 1, vidx + 2};
		vidx += 3;
		p++;
	}
	object->stats.vertex = object->stats.faces * 3;
	ft_mfree(5, object->items, object->uv, object->fuv, object->normals,
		object->fnormals);
	object->items = items;
	return (0);
}
