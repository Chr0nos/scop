/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/08 13:54:52 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

static int				parse_real(const char *filepath, t_vertex_pack *pack)
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
		else if ((!ft_strncmp(line, "f \\S", 2)) && (parse_face(&line[2], &tp)))
			;
		else if (ft_sscanf(line, "vt \\S%f %f", &tp.uv->x, &tp.uv->y) == 2)
			tp.uv++;
		else if (ft_sscanf(line, "vn \\S%d %d %d", &tp.normals->x,
					&tp.normals->y, &tp.normals->z) == 3)
			tp.normals++;
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

static t_vertex_pack	*parse_post_process(t_vertex_pack *pack)
{
	const int			mv = (int)pack->stats.vertex - 1;
	t_v3i				*face;
	size_t				p;

	ft_putendl("fixing negatives indexes");
	p = 0;
	while (p < pack->stats.faces)
	{
		face = &pack->faces[p];

		*face = geo_subv3i(*face, (t_v3i){1, 1, 1});
		if (face->x < 0)
			face->x = mv - face->x;
		if (face->y < 0)
			face->y = mv - face->y;
		if (face->z < 0)
			face->z = mv - face->z;
		p++;
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
	pack->vertex = (t_v3f*)((size_t)pack + sizeof(t_vertex_pack));
	pack->uv = (t_v2f*)((size_t)pack->vertex + (sizeof(t_v3f) * stats->vertex));
	pack->flags = (unsigned char *)((size_t)pack->uv +
		(sizeof(t_v2f) * stats->uv));
	pack->faces = (t_v3i*)((size_t)pack->flags + (sizeof(char) * stats->faces));
	pack->fuv = (t_v3i*)((size_t)pack->faces + (sizeof(t_v3i) * stats->faces));
	pack->normals = (t_v3i*)((size_t)pack->fuv + (sizeof(t_v3i) * stats->uv));
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
