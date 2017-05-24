/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/24 00:57:24 by snicolet         ###   ########.fr       */
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

static void				parse_vertex(t_vertex_pack *tp, const int ret,
	unsigned int color)
{
	if (ret == 4)
		color_load(&tp->vertex_items->color, color);
	else
		tp->vertex_items->color = (t_v4f){0.2f, 0.2f, 0.2f, 1.0f};
	tp->vertex_items++;
	tp->vertex++;
}

static int				parse_real(const char *filepath, t_vertex_pack *pack)
{
	int				fd;
	char			*line;
	unsigned int	color;
	t_vertex_pack	tp;
	int				ret;
	t_vertex_item	*x;

	x = pack->vertex_items;
	ft_memcpy(&tp, pack, sizeof(t_vertex_pack));
	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
	while (ft_get_next_line(fd, &line) > 0)
	{
		if ((ret = ft_sscanf(line, "v \\S%f \\S%f \\S%f \\S%x", &tp.vertex->x,
				&tp.vertex->y, &tp.vertex->z, &color)) >= 3)
			parse_vertex(&tp, ret, color);
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(&line[2], &tp)))
			;
		else if (ft_sscanf(line, "vt \\S%f \\S%f", &tp.uv->x, &tp.uv->y) == 2)
			tp.uv++;
		else if (ft_sscanf(line, "vn \\S%f \\S%f \\S%f", &x->normal.x,
				&x->normal.y, &x->normal.z) == 3)
			x++;
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

static t_vertex_pack	*parse_post_process(t_vertex_pack *pack)
{
	const int			mv = (int)pack->stats.vertex - 1;
	size_t				p;

	ft_putendl("fixing negatives indexes");
	p = pack->stats.faces;
	while (p--)
	{
		pack->faces[p] = geo_subv3i(pack->faces[p], (t_v3i){1, 1, 1});
		pack->fuv[p] = geo_subv3i(pack->fuv[p], (t_v3i){1, 1, 1});
		parse_fixnegi((int*)&pack->faces[p], 3, mv);
		parse_fixnegi((int*)&pack->fuv[p], 3, (int)(pack->stats.vertex - 1));
	}
	ft_putendl("fix done");
	return (pack);
}

static t_vertex_pack	*parse_setptrs(t_vertex_pack *pack, t_obj_stats *stats)
{
	if (!pack)
	{
		ft_putstr_fd("error: failed to alllocate memory ! we are doomed!\n", 2);
		return (NULL);
	}
	pack->vertex_items = (t_vertex_item*)((size_t)pack + sizeof(t_vertex_pack));
	pack->vertex = (t_v3f*)((size_t)pack->vertex_items +
		(sizeof(t_vertex_item) * stats->vertex));
	pack->uv = (t_v2f*)((size_t)pack->vertex + (sizeof(t_v3f) * stats->vertex));
	pack->flags = (unsigned char *)((size_t)pack->uv +
		(sizeof(t_v2f) * stats->uv));
	pack->faces = (t_v3i*)((size_t)pack->flags + (sizeof(char) * stats->faces));
	pack->fuv = (t_v3i*)((size_t)pack->faces + (sizeof(t_v3i) * stats->faces));
	pack->normals = (t_v3f*)((size_t)pack->fuv + (sizeof(t_v3i) * stats->uv));
	return (pack);
}

t_vertex_pack			*parse_obj(const char *filepath)
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
	if (!(pack = parse_setptrs(malloc(stats.fullsize), &stats)))
		return (NULL);
	pack->stats = stats;
	ft_bzero(pack->flags, stats.faces);
	if ((parse_real(filepath, pack) < 0) && (ft_mfree(1, pack)))
		return (NULL);
	pack->center = geo_center_v3(pack->vertex, stats.vertex);
	fixcenter(pack);
	return (parse_post_process(pack));
}
