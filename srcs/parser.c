/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/01 17:02:30 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#define CHARTOF (1.0f / (float)0xff)

void					color_load(t_v4f *target, const unsigned int color)
{
	*target = (t_v4f){
		.x = (float)((color & 0xff000000) >> 24) * CHARTOF,
		.y = (float)((color & 0x00ff0000) >> 16) * CHARTOF,
		.z = (float)((color & 0x0000ff00) >> 8) * CHARTOF,
		.w = (float)((color & 0x000000ff)) * CHARTOF
	};
}

static void				parse_vertex(t_vertex_pack *pack, const int ret,
	unsigned int color)
{
	if (ret == 4)
		color_load(&pack->items->color, color);
	pack->items++;
	pack->stats.current_vertex++;
}

static int				parse_real(const char *filepath, t_vertex_pack *pack)
{
	int				fd;
//	char			line[256];
	char			*line;
	unsigned int	color;
	int				ret;

	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
//	while (ft_gl(line, fd, sizeof(line)) > 0)
	while (ft_get_next_line(fd, &line) > 0)
	{
		if ((ret = ft_sscanfq(line, "v \\S%f \\S%f \\S%f \\S#%x",
				&pack->items->position.x, &pack->items->position.y,
				&pack->items->position.z, &color)) >= 3)
			parse_vertex(pack, ret, color);
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(&line[2], pack)))
			;
		else if (ft_sscanfq(line, "vt \\S%f \\S%f", &pack->uv->x,
					&pack->uv->y) == 2)
			pack->uv++;
		else if (ft_sscanfq(line, "vn \\S%f \\S%f \\S%f", &pack->normals->x,
				&pack->normals->y, &pack->normals->z) == 3)
			pack->normals++;
		free(line);
	}
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
		if ((pack->faces[p].x > mv) || (pack->faces[p].y > mv) ||
				(pack->faces[p].z > mv))
		{
			ft_printf("%lu -> %d %d %d\n", p, pack->faces[p].x, pack->faces[p].y,
				pack->faces[p].z);
			ft_dprintf(2, "error: invalid face in obj file !\n");
			return (1);
		}
		parse_fixnegi((int*)&pack->fuv[p], 3, mv);
		parse_fixnegi((int*)&pack->fnormals[p], 3, mv);
	}
	fixcenter(pack);
	parse_duplicate(pack);
	ft_putendl("fix done");
	return (0);
}

static t_vertex_pack	*parse_setptrs(t_vertex_pack *pack)
{
	const t_obj_stats	*stats = &pack->stats;

	pack->items = ft_memalloc(sizeof(t_vertex_item) * stats->vertex);
	pack->faces = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->fuv = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->uv = ft_memalloc(sizeof(t_v2f) * stats->uv);
	pack->fnormals = ft_memalloc(sizeof(t_v3i) * stats->faces);
	pack->normals = ft_memalloc(sizeof(t_v3f) * stats->normal);
	if ((!pack->items) || (!pack->faces) || (!pack->fuv) ||
		(!pack->uv) || (!pack->fnormals) || (!pack->normals))
	{
		ft_mfree(6, pack->items, pack->faces, pack->fuv, pack->uv,
				pack->normal, pack->fnormals);
		ft_dprintf(2, "error: failed to malloc !\n");
		return (NULL);
	}
	return (pack);
}

int			parse_obj(t_vertex_pack *pack, const char *filepath)
{
	t_vertex_pack		origin;

	pack->stats = parser_count(filepath);
	if (pack->stats.vertex + pack->stats.faces == 0)
	{
		ft_dprintf(2, "error: no faces or vertex to display\n");
		return (1);
	}
	if (!(parse_setptrs(pack)))
		return (1);
	origin = *pack;
	if ((parse_real(filepath, pack) < 0) && (ft_mfree(1, pack->items)))
		return (1);
	*pack = origin;
	return (parse_post_process(pack));
}
