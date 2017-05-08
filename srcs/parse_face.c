/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_face.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 13:34:37 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/08 13:55:24 by snicolet         ###   ########.fr       */
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

int						parse_face(const char *line, t_vertex_pack *pack)
{
	t_v3i	f;
	t_v3i	uv;
	int		p;
	int		uvc;
	int		ret;

	f = (t_v3i){0, 0, 0};
	p = -1;
	uvc = 0;
	while ((line) && (*line) && (++p < 3))
	{
		if ((ret = ft_sscanf(line, "\\S%d%N/%d%N/%*d%N",
				&((int*)&f)[p], &line, &((int*)&uv)[p], &line, &line)) >= 3)
			uvc++;
		else if (ret < 1)
			return (0);
	}
	add_face(pack, f);
	if (uvc == 3)
		add_uv(pack, (t_v3i){uv.x - 1, uv.y - 1, uv.z - 1});
	pack->flags++;
	if ((line) && (*line) &&
		((ret = ft_sscanf(line, "\\S%d/%d/%*d", &p, &uv.y)) >= 1))
	{
		add_face(pack, (t_v3i){f.x - 0, f.z - 0, p - 1});
		if (uvc + ret >= 5)
			add_uv(pack, (t_v3i){uv.x - 1, uv.z - 1, uv.y - 1});
		pack->flags++;
		return (2);
	}
	return (1);
}
