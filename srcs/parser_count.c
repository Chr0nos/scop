/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/20 02:09:01 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/22 16:14:21 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <fcntl.h>
#include <stdlib.h>

#define F4X3 "f \\S%*d/%*d/%*d \\S%*d/%*d/%*d \\S%*d/%*d/%*d \\S%*d/%*d/%*d"
#define F4X2 "f \\S%*d/%*d \\S%*d/%*d \\S%*d/%*d \\S%*d/%*d"
#define F4X2B "f \\S%*d//%*d \\S%*d//%*d \\S%*d//%*d"
#define F4X1 "f \\S%*d \\S%*d \\S%*d \\S%*d"

static int		parser_count_faces(const char *line, t_obj_stats *stats)
{
	int		ret;

	if ((ret = ft_sscanf(line, F4X1)) >= 3)
		stats->faces += (unsigned)ret - 2;
	else if ((ret = ft_sscanf(line, F4X3)) >= 9)
		stats->faces += (ret == 9) ? 1 : 2;
	else if ((ret = ft_sscanf(line, F4X2)) >= 6)
		stats->faces += (ret == 6) ? 1 : 2;
	else if ((ret = ft_sscanf(line, F4X2B)) == 4)
		stats->faces += 1;
	else
		return (0);
	return (1);
}

static int		parse_line_error(const char *line)
{
	if ((*line == '#') || (!(*line)))
		return (0);
	while (*line)
	{
		if ((!ft_isprint((int)*line)) && (*line != '\n') && (*line != 13))
		{
			ft_dprintf(2, "error: invalid file detected\n");
			return (1);
		}
		line++;
	}
	return (0);
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
	while ((ft_get_next_line(fd, &line) > 0) && (!parse_line_error(line)))
	{
		if ((ret = ft_sscanf(line, "v \\S%*f \\S%*f \\S%*f")) == 3)
			stats.vertex++;
		else if (parser_count_faces(line, &stats))
			;
		else if ((ret = ft_sscanf(line, "vt \\S%*f \\S%*f")) == 2)
			stats.uv++;
		else if ((ret = ft_sscanf(line, "vn \\S%*f \\S%*f \\S%*f")) == 3)
			stats.normal++;
		free(line);
	}
	free(line);
	close(fd);
	ft_printf("stats: vertex: %lu / faces: %lu / uv: %lu / normal: %lu\n",
		stats.vertex, stats.faces, stats.uv, stats.normal);
	return (stats);
}
