/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 13:34:37 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 03:56:33 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"

/*
** new implementation of faces parsing, the idea is to be more efficiant and
** be abble to parse normals withous makking the whole thing a gaz factory like
** the precedent faces parser
** idx.x = vertex index
** idx.y = uv index
** idx.z = normal index
** ret possibles values:
** x/y/z = 3
** x/y   = 2
** x     = 1
** if 0 we are at the end of the line
** the "p" variable is used to know wich vertex we are working on
** a face is composed by 3 vertex, so "p" can be 0/1/2 for x y z vertexes
** returns: the amount of faces created (0, 1 or 2)
*/

static int			fix_item(int *x, const int max)
{
	if (*x < 0)
		*x = max + *x;
	else
	{
		(*x)--;
		if (*x < 0)
			*x = 0;
	}
	return (*x);
}

static int			parse_face_extra(const char *line, struct s_object *object,
	const t_v3i *history)
{
	int		ret;
	t_v3i	idx;

	ret = ft_sscanfq(line, "\\S%d/%d/%d", &idx.x, &idx.y, &idx.z);
	if (ret <= 0)
		return (1);
	if (fix_item(&idx.x, (int)object->stats.current_vertex) < 0)
		return (1);
	*(object->faces++) = (t_v3i){history[0].x, history[2].x, idx.x};
	*(object->fuv++) = (t_v3i){history[0].y, history[2].y, idx.y - 1};
	*(object->fnormals++) = (t_v3i){history[0].z, history[2].z, idx.z - 1};
	return (2);
}

static void			fix_idx(t_v3i *idx, const t_v3i max)
{
	fix_item(&idx->x, max.x);
	fix_item(&idx->y, max.y);
	fix_item(&idx->z, max.z);
}

/*
** maximals are used to be abble to parse negatives indexes
** we need to know the current maximal value
*/

static t_v3i		get_maximals(const t_obj_stats *stats)
{
	return ((t_v3i){
		.x = (int)stats->current_vertex,
		.y = (int)stats->current_uv,
		.z = (int)stats->current_normal
	});
}

/*
** called on each face encountred in the .obj file
** the line must start with: "f "
** return the amount of faces found on the line
*/

int					parse_face(const char *line, struct s_object *object)
{
	size_t			p;
	int				ret;
	t_v3i			idx;
	t_v3i			history[3];
	const t_v3i		max = get_maximals(&object->stats);

	p = 0;
	idx = (t_v3i){0, 0, 0};
	while ((p < 3) && (line) && (*line))
	{
		if ((ret = ft_sscanfq(line, "\\S%d%N/%d%N/%d%N",
				&idx.x, &line, &idx.y, &line, &idx.z, &line)) < 0)
			return (0);
		fix_idx(&idx, max);
		((int*)object->faces)[p] = idx.x;
		((int*)object->fuv)[p] = idx.y;
		((int*)object->fnormals)[p] = idx.z;
		history[p++] = idx;
	}
	object->faces++;
	object->fuv++;
	object->fnormals++;
	if ((p == 3) && (*line))
		return (parse_face_extra(line, object, history));
	return (1);
}
