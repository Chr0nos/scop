/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/24 15:41:30 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static t_vertex_pack	*makepack(const size_t points)
{
	t_vertex_pack	*pack;
	size_t			rsize;

	rsize = points * (sizeof(t_v3f) + sizeof(t_v2f) + sizeof(unsigned char) +
		sizeof(t_vertex_pack));
	if (!(pack = malloc(rsize)))
		return (NULL);
	pack->vertex = (t_v3f*)((unsigned long) + sizeof(t_vertex_pack));
	pack->uv = (t_v2f*)((unsigned long)pack->vertex + (sizeof(t_v3f) * points));
	pack->flags = (unsigned char*)((unsigned long)pack->uv + sizeof(t_v2f) *
		points);
	pack->points = points;
	return (pack);
}

//phase 1 creer la liste chainee avec toutes les infos et rien de plus
//phase 2 passer la liste en tableau
//phase 3 recuperer la taille du tout
//phase 4 alouer la memoire avec makepack
//phase 5 remplire le pack
//phase 6 netoyer tout le bazard que j aurais mis

static t_vertex_pack	*load_obj_real(const int fd)
{
	t_list			*lst;
	char			*line;
	size_t			points;

	(void)makepack;
	lst = NULL;
	points = 0;
	//phase 1
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (line[0] == 'v')
		{
			points++;
			ft_printf("[%3d] - %s\n", points++, line);
			ft_lstadd(&lst, ft_lstnewlink(line, 0));
		}
	}
	//phase 2
	
	//phase 6
	ft_lstdel(&lst, ft_lstpulverisator);
	return (NULL);
}

t_vertex_pack			*load_obj(const char *filepath)
{
	int				fd;
	t_vertex_pack	*pack;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	pack = load_obj_real(fd);
	close(fd);
	return (pack);
}
