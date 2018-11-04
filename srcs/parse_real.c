/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_real.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 14:57:27 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 04:18:57 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <fcntl.h>
#include <unistd.h>

static void				parse_vertex(struct s_object *object, const int ret,
	unsigned int color)
{
	if (ret == 4)
		color_load(&object->items->color, color);
	object->items++;
	object->stats.current_vertex++;
}

static inline void		parse_uv(struct s_object *object)
{
	object->uv++;
	object->stats.current_uv++;
}

static inline void		parse_normal(struct s_object *object)
{
	object->normals++;
	object->stats.current_normal++;
}

static inline int		parse_real_clean(char *line, const int fd)
{
	free(line);
	close(fd);
	return (0);
}

int						parse_real(const char *filepath, struct s_object *object)
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
				&object->items->position.x, &object->items->position.y,
				&object->items->position.z, &color)) >= 3)
			parse_vertex(object, ret, color);
		else if ((!ft_strncmp(line, "f ", 2)) && (parse_face(&line[2], object)))
			;
		else if (ft_sscanfq(line, "vt \\S%f \\S%f", &object->uv->x,
					&object->uv->y) == 2)
			parse_uv(object);
		else if (ft_sscanfq(line, "vn \\S%f \\S%f \\S%f", &object->normals->x,
				&object->normals->y, &object->normals->z) == 3)
			parse_normal(object);
		free(line);
	}
	return (parse_real_clean(line, fd));
}
