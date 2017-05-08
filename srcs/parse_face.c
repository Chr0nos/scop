/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 13:34:37 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/08 14:41:55 by snicolet         ###   ########.fr       */
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

	add_face(pack, (t_v3i){face.x, face.z, p - 1});
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
