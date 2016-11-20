/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/20 02:09:01 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/20 02:10:19 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <fcntl.h>
#include <stdlib.h>

#define PATTERN_F4X3 "f %*d/%*d/%*d %*d/%*d/%*d %*d/%*d/%*d %*d/%*d/%*d"
#define PATTERN_F4X2 "f %*d/%*d %*d/%*d %*d/%*d %*d/%*d"
#define PATTERN_F4X1 "f %*d %*d %*d %*d"

static int		parser_count_faces(const char *line, t_obj_stats *stats)
{
	int		ret;

	if ((ret = ft_sscanf(line, PATTERN_F4X1)) >= 3)
		stats->faces += (unsigned)ret - 2;
	else if ((ret = ft_sscanf(line, PATTERN_F4X3)) >= 9)
		stats->faces += (ret == 9) ? 1 : 2;
	else if ((ret = ft_sscanf(line, PATTERN_F4X2)) >= 6)
		stats->faces += (ret == 6) ? 1 : 2;
	else
		return (0);
	return (1);
}

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
		else if (parser_count_faces(line, &stats))
			;
		else if ((ret = ft_sscanf(line, "vt %*f %*f")) == 2)
			stats.uv++;
		free(line);
	}
	close(fd);
	ft_printf("stats: vertex: %lu / faces: %lu / uv: %lu\n",
		stats.vertex, stats.faces, stats.uv);
	return (stats);
}
