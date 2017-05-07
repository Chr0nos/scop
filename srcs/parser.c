/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/07 21:06:53 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static void		add_uv(t_vertex_pack *pack, const t_v3i uv)
{
	*(pack->fuv++) = uv;
	*pack->flags |= FLAG_UV;
}

static void		add_face(t_vertex_pack *pack, const t_v3i f)
{
	*(pack->faces++) = f;
}

static int		parse_face(const char *line, t_vertex_pack *pack)
{
	t_v3i	f;
	t_v3i	uv;
	int		p;
	int		uvc;
	int		ret;

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
	add_face(pack, (t_v3i){f.x - 1, f.y - 1, f.z - 1});
	if (uvc == 3)
		add_uv(pack, (t_v3i){uv.x - 1, uv.y - 1, uv.z - 1});
	pack->flags++;
	if ((line) && (*line) &&
		((ret = ft_sscanf(line, "\\S%d/%d/%*d", &p, &uv.y)) >= 1))
	{
		add_face(pack, (t_v3i){f.x - 1, f.z - 1, p - 1});
		if (uvc + ret >= 5)
			add_uv(pack, (t_v3i){uv.x - 1, uv.z - 1, uv.y - 1});
		pack->flags++;
		return (2);
	}
	return (1);
}

static int		parse_real(const char *filepath, t_vertex_pack *pack)
{
	int				fd;
	char			*line;
	t_vertex_pack	tp;

	ft_memcpy(&tp, pack, sizeof(t_vertex_pack));
	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (ft_sscanf(line, "v %f %f %f", &tp.vertex->x, &tp.vertex->y,
				&tp.vertex->z) == 3)
			tp.vertex++;
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(&line[2], &tp)))
			;
		else if (ft_sscanf(line, "vt %f %f", &tp.uv->x, &tp.uv->y) == 2)
			tp.uv++;
		else if (ft_sscanf(line, "vn %d %d %d", &tp.normals->x,
					&tp.normals->y, &tp.normals->z) == 3)
			tp.normals++;
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

t_vertex_pack	*parse_obj(const char *filepath)
{
	t_obj_stats			stats;
	t_vertex_pack		*pack;

	stats = parser_count(filepath);
	if (stats.vertex + stats.faces == 0)
	{
		ft_dprintf(2, "error: no faces or vertex to display\n");
		return (NULL);
	}
	stats.fullsize = parse_calc_size(&stats);
	ft_printf("trying to alllocate: %lu bytes\n", stats.fullsize);
	if (!(pack = malloc(stats.fullsize)))
		return (NULL);
	pack->vertex = (t_v3f*)((size_t)pack + sizeof(t_vertex_pack));
	pack->uv = (t_v2f*)((size_t)pack->vertex + (sizeof(t_v3f) * stats.vertex));
	pack->flags = (unsigned char *)((size_t)pack->uv +
		(sizeof(t_v2f) * stats.uv));
	ft_bzero(pack->flags, stats.faces);
	pack->faces = (t_v3i*)((size_t)pack->flags + (sizeof(char) * stats.faces));
	pack->fuv = (t_v3i*)((size_t)pack->faces + (sizeof(t_v3i) * stats.faces));
	pack->normals = (t_v3i*)((size_t)pack->fuv + (sizeof(t_v3i) * stats.uv));
	pack->stats = stats;
	if ((parse_real(filepath, pack) < 0) && (ft_mfree(1, pack)))
		return (NULL);
	pack->center = geo_center_v3(pack->vertex, stats.vertex);
	fixcenter(pack);
	return (pack);
}
