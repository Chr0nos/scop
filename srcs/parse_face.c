/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 13:34:37 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/31 16:02:31 by snicolet         ###   ########.fr       */
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

static int			parse_face_extra(const char *line, t_vertex_pack *pack,
	const t_v3i *history)
{
	int		ret;
	t_v3i	idx;

	ret = ft_sscanfq(line, "\\S%d/%d/%d", &idx.x, &idx.y, &idx.z);
	if (ret <= 0)
		return (1);
	if (idx.x < 0)
	{
		idx.x = (int)pack->stats.current_vertex - idx.x;
		if (idx.x < 0)
			return (1);
	}
	*(pack->faces++) = (t_v3i){history[0].x, history[2].x, idx.x - 1};
	*(pack->fuv++) = (t_v3i){history[0].y, history[2].y, idx.y - 1};
	*(pack->fnormals++) = (t_v3i){history[0].z, history[2].z, idx.z - 1};
	return (2);
}

static void			fix_idx(t_v3i *idx, int max)
{
	if (idx->x < 0)
		idx->x = max - idx->x;
	if (idx->y < 0)
		idx->y = max - idx->y;
	if (idx->z < 0)
		idx->z = max - idx->z;
	*idx = (t_v3i){idx->x - 1, idx->y - 1, idx->z - 1};
	if ((idx->x < 0) || (idx->y < 0) || (idx->z < 0))
		*idx = (t_v3i){0, 0, 0};
}

int					parse_face(const char *line, t_vertex_pack *pack)
{
	size_t		p;
	int			ret;
	t_v3i		idx;
	t_v3i		history[3];

	p = 0;
	idx = (t_v3i){0, 0, 0};
	while ((p < 3) && (line) && (*line))
	{
		ret = ft_sscanfq(line, "\\S%d%N/%d%N/%d%N",
				&idx.x, &line, &idx.y, &line, &idx.z, &line);
		if ((ret < 0) || (idx.x < 0))
			return (0);
		fix_idx(&idx, (int)pack->stats.current_vertex);	
		((int*)pack->faces)[p] = idx.x;
		((int*)pack->fuv)[p] = idx.y;
		((int*)pack->fnormals)[p] = idx.z;
		history[p++] = idx;
	}
	pack->faces++;
	pack->fuv++;
	pack->fnormals++;
	if ((p == 3) && (*line))
		return (parse_face_extra(line, pack, history));
	return (1);
}
