/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 16:43:47 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static void				color_load(t_v4f *target, const unsigned int color)
{
	*target = (t_v4f){
		.x = (float)((color & 0xff000000) >> 24) / (float)0xff,
		.y = (float)((color & 0x00ff0000) >> 16) / (float)0xff,
		.z = (float)((color & 0x0000ff00) >> 8) / (float)0xff,
		.w = (float)((color & 0x000000ff)) / (float)0xff
	};
}

static void				parse_vertex(t_vertex_ptrs *ptrs, const int ret,
	unsigned int color)
{
	static int				color_index = 0;
	const unsigned int		colors[6] = {
		0xff0000ff,
		0xffff00ff,
		0x0000ffff,
		0xff00ffff,
		0xb0b0b0ff,
		0x000000ff
	};
	if (ret == 4)
		color_load(&ptrs->vertex->color, color);
	else
	{
		color_load(&ptrs->vertex->color, colors[color_index++]);
		if (color_index > 5)
			color_index = 0;
	}
	ptrs->vertex++;
}

static int				parse_real(const char *filepath, t_vertex_pack *pack)
{
	int				fd;
	char			*line;
	unsigned int	color;
	int				ret;
	t_vertex_ptrs	ptrs;

	ptrs = (t_vertex_ptrs){pack->uv, pack->items, pack->items};
	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
	while (ft_get_next_line(fd, &line) > 0)
	{
		if ((ret = ft_sscanf(line, "v \\S%f \\S%f \\S%f \\S%x",
				&ptrs.vertex->position.x,
				&ptrs.vertex->position.y,
				&ptrs.vertex->position.z, &color)) >= 3)
			parse_vertex(&ptrs, ret, color);
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(&line[2], pack)))
			;
		else if (ft_sscanf(line, "vt \\S%f \\S%f", &ptrs.uv->x,
					&ptrs.uv->y) == 2)
			ptrs.uv++;
		else if (ft_sscanf(line, "vn \\S%f \\S%f \\S%f",
				&ptrs.normal->normal.x,
				&ptrs.normal->normal.y,
				&ptrs.normal->normal.z) == 3)
			ptrs.normal++;
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

static void				parse_fixnegi(int *data, unsigned int size, int max)
{
	while (size--)
		if (data[size] < 0)
			data[size] = max;
}

static int				parse_post_process(t_vertex_pack *pack)
{
	const int			mv = (int)pack->stats.vertex - 1;
	size_t				p;

	ft_putendl("fixing negatives indexes");
	p = pack->stats.faces;
	while (p--)
	{
		pack->faces[p] = geo_subv3i(pack->faces[p], (t_v3i){1, 1, 1});
		if ((pack->faces[p].x > mv) || (pack->faces[p].y > mv) ||
				(pack->faces[p].z > mv))
		{
			ft_dprintf(2, "error: invalid face in obj file !\n");
			return (1);
		}
		pack->fuv[p] = geo_subv3i(pack->fuv[p], (t_v3i){1, 1, 1});
		parse_fixnegi((int*)&pack->faces[p], 3, mv);
		parse_fixnegi((int*)&pack->fuv[p], 3, (int)(pack->stats.vertex - 1));
	}
	fixcenter(pack);
	ft_putendl("fix done");
	return (0);
}

static t_vertex_pack	*parse_setptrs(t_vertex_pack *pack, t_obj_stats *stats)
{
	pack->items = ft_memalloc(sizeof(t_vertex_item) * stats->vertex);
	pack->faces = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->fuv = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->flags = ft_memalloc(sizeof(char) * stats->faces);
	pack->uv = ft_memalloc(sizeof(t_v2f) * stats->uv);
	if ((!pack->items) || (!pack->faces) || (!pack->fuv) ||
			(!pack->flags) || (!pack->uv))
	{
		ft_mfree(5, pack->items, pack->faces, pack->fuv, pack->flags, pack->uv);
		ft_dprintf(2, "error: failed to malloc !\n");
		return (NULL);
	}
	return (pack);
}

int			parse_obj(t_vertex_pack *pack, const char *filepath)
{
	t_obj_stats			stats;
	t_vertex_pack		origin;

	stats = parser_count(filepath);
	if (stats.vertex + stats.faces == 0)
	{
		ft_dprintf(2, "error: no faces or vertex to display\n");
		return (1);
	}
	if (!(parse_setptrs(pack, &stats)))
		return (1);
	pack->stats = stats;
	origin = *pack;
	if ((parse_real(filepath, pack) < 0) && (ft_mfree(1, pack->items)))
		return (1);
	*pack = origin;
	return (parse_post_process(pack));
}
