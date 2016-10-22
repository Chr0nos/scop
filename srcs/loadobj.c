/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/22 13:38:22 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static t_vertex_pack	*load_obj_real(const int fd)
{
	t_vertex_pack	*pack;
	char			*line;

	if (!(pack = malloc(sizeof(t_vertex_pack))))
		return (NULL);
	pack->points = 0;
	pack->pts = NULL;
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (line[0] == 'v')
		{
			ft_printf("[%3d] - %s\n", pack->points++, line);
		}
		free(line);
	}
	return (pack);
}

t_vertex_pack			*load_obj(const char *filepath)
{
	int		fd;
	t_vertex_pack	*pack;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	pack = load_obj_real(fd);
	close(fd);
	return (pack);
}
