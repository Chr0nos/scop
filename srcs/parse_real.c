/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_real.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 15:25:01 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/04 17:21:13 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <fcntl.h>
#include <unistd.h>

static void				parse_vertex(t_vertex_pack *pack, const int ret,
	unsigned int color)
{
	if (ret == 4)
		color_load(&pack->items->color, color);
	pack->items++;
	pack->stats.current_vertex++;
}

static inline void		parse_uv(t_vertex_pack *pack)
{
	pack->uv++;
	pack->stats.current_uv++;
}

static inline void		parse_normal(t_vertex_pack *pack)
{
	pack->normals++;
	pack->stats.current_normal++;
}

int						parse_real(const char *filepath, t_vertex_pack *pack)
{
	int				fd;
	char			*line;
	unsigned int	color;
	int				ret;

	if ((fd = open(filepath, O_RDONLY)) <= 0)
		return (-1);
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
			parse_uv(pack);
		else if (ft_sscanfq(line, "vn \\S%f \\S%f \\S%f", &pack->normals->x,
				&pack->normals->y, &pack->normals->z) == 3)
			parse_normal(pack);
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

