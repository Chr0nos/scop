/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/22 13:15:30 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static t_pt_c		*load_obj_real(const int fd)
{
	int			size;
	t_pt_c		*pts;
	char		*line;

	size = 0;
	pts = NULL;
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (line[0] == 'v')
		{
			ft_printf("[%3d] - %s\n", size++, line);
		}
		free(line);
	}
	return (pts);
}

t_pt_c				*load_obj(const char *filepath)
{
	int		fd;
	t_pt_c	*pts;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	pts = load_obj_real(fd);
	close(fd);
	return (pts);
}
