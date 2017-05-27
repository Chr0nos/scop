/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 13:34:37 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/27 16:11:50 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"

static void				add_uv(t_vertex_pack *pack, const t_v3i uv)
{
	*(pack->fuv++) = uv;
	*pack->flags |= FLAG_UV;
}

static void				add_face(t_vertex_pack *pack, const t_v3i f)
{
	*(pack->faces++) = f;
}

static int				parse_face_x4(t_vertex_pack *pack, t_v3i uv, t_v3i face,
	const int *cheat)
{
	const int		p = cheat[0];
	const int		ret = cheat[1];
	const int		uvc = cheat[2];

	add_face(pack, (t_v3i){face.x, face.z, p});
	if (uvc + ret >= 5)
		add_uv(pack, (t_v3i){uv.x, uv.z, uv.y});
	pack->flags++;
	return (2);
}

static void				parse_face_init(t_v3i *face, t_v3i *uv, int *p)
{
	*p = -1;
	*face = (t_v3i){0, 0, 0};
	*uv = (t_v3i){0, 0, 0};
}

int						parse_face(const char *line, t_vertex_pack *pack)
{
	t_v3i	face;
	t_v3i	uv;
	int		p;
	int		uvc;
	int		ret;

	parse_face_init(&face, &uv, &p);
	uvc = 0;
	while ((line) && (*line) && (++p < 3))
	{
		if ((ret = ft_sscanf(line, "\\S%d%N/%d%N/%*d%N",
				&((int*)&face)[p], &line, &((int*)&uv)[p], &line, &line)) >= 3)
			uvc++;
		else if (ret < 1)
			return (0);
	}
	add_face(pack, face);
	if (uvc == 3)
		add_uv(pack, uv);
	pack->flags++;
	if ((line) && (*line) &&
		((ret = ft_sscanf(line, "\\S%d/%d/%*d", &p, &uv.y)) >= 1))
		return (parse_face_x4(pack, uv, face, (const int[3]){p, ret, uvc}));
	return (1);
}

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
** note: we don't care about pack->flags anymore, this part will be removed soon
** returns: the amount of faces created (0, 1 or 2)
*/

int					parse_face_ng(const char *line, t_vertex_pack *pack)
{
	size_t		p;
	int			ret;
	t_v3i		idx;

	p = 0;
	ret = 1;
	idx = (t_v3i){0, 0, 0};
	while ((p < 2) && (ret > 0) && (line) && (*line))
	{
		ret = ft_sscanf(line, "\\S%d%N/%d%N/%d%N",
				&idx.x, &line, &idx.y, &line, &idx.z, &line);
		((int*)pack->faces++)[p] = idx.x;
		((int*)pack->fuv++)[p] = idx.y;
		((int*)pack->fnormals++)[p] = idx.z;
		p++;
	}
	return ((ret < 1) ? 0 : 1);
}
