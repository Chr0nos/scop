/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 15:47:56 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/17 01:03:40 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>

t_obj_stats		parser_count(const char *filepath)
{
	t_obj_stats			stats;
	int					fd;
	int					ret;
	char				*line;

	ft_bzero(&stats, sizeof(t_obj_stats));
	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (stats);
	while (ft_get_next_line(fd, &line) > 0)
	{
		if ((ret = ft_sscanf(line, "v %*f %*f %*f")) == 3)
			stats.vertex++;
		else if ((ret = ft_sscanf(line, "f %*d %*d %*d %*d")) >= 3)
			stats.faces += (unsigned)ret - 2;
		else if ((ret = ft_sscanf(line, "vt %*f %*f")) == 2)
			stats.uv++;
		free(line);
	}
	close(fd);
	ft_printf("stats: vertex: %lu / faces: %lu / uv: %lu\n",
		stats.vertex, stats.faces, stats.uv);
	return (stats);
}

static int		parse_face(const char *line, t_v3i **face)
{
	int		extra;
	int		ret;
	t_v3i	*f;

	f = *face;
	ret = ft_sscanf(line, "f %d %d %d %d", &f->x, &f->y, &f->z, &extra);
	if (ret >= 3)
	{
		f[0] = (t_v3i){f->x - 1, f->y - 1, f->z - 1};
		*face += 1;
	}
	if (ret == 4)
	{
		f[1] = (t_v3i){f->y, f->z, extra - 1};
		*face += 1;
		return (2);
	}
	return (ret >= 3);
}

static int		parse_real(const char *filepath, t_vertex_pack *pack)
{
	int		fd;
	char	*line;
	t_v3f	*vertex;
	t_v3i	*faces;
	t_v2f	*uv;

	vertex = pack->vertex;
	faces = pack->faces;
	uv = pack->uv;
	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (ft_sscanf(line, "v %f %f %f", &vertex->x, &vertex->y,
				&vertex->z) == 3)
			vertex++;
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(line, &faces)))
			;
		else if (ft_sscanf(line, "vt %f %f", &uv->x, &uv->y) == 2)
			uv++;
		free(line);
	}
	close(fd);
	return (0);
}

t_vertex_pack	*parse_obj(const char *filepath)
{
	t_obj_stats			stats;
	t_vertex_pack		*pack;
	size_t				size;

	stats = parser_count(filepath);
	size = sizeof(t_vertex_pack) + (sizeof(t_v3f) * stats.vertex) +
		(sizeof(t_v2f) * stats.uv) + (sizeof(unsigned char) * stats.vertex) +
		(sizeof(t_v3i) * stats.faces);
	ft_printf("trying to alllocate: %lu bytes\n", size);
	if (!(pack = malloc(size)))
		return (NULL);
	pack->vertex = (t_v3f*)((size_t)pack + sizeof(t_vertex_pack));
	pack->uv = (t_v2f*)((size_t)pack->vertex + (sizeof(t_v3f) * stats.vertex));
	pack->flags = (unsigned char *)((size_t)pack->uv +
		(sizeof(t_v2f) * stats.uv));
	ft_bzero(pack->flags, stats.vertex);
	pack->faces = (t_v3i*)((size_t)pack->flags + (sizeof(char) * stats.vertex));
	pack->stats = stats;
	if (parse_real(filepath, pack) < 0)
	{
		free(pack);
		return (NULL);
	}
	pack->center = geo_center_v3(pack->vertex, stats.vertex);
	return (pack);
}
