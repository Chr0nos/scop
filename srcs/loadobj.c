/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 21:08:11 by snicolet         ###   ########.fr       */
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

	rsize = (points *
		((sizeof(t_v3f) + sizeof(t_v2f) + sizeof(unsigned char))) +
		sizeof(t_vertex_pack));
	ft_printf("alloc size: %d\n", (int)rsize);
	if (!(pack = malloc(rsize)))
		return (NULL);
	pack->vertex = (t_v3f*)((unsigned long)pack + sizeof(t_vertex_pack));
	pack->uv = (t_v2f*)((unsigned long)pack->vertex + (sizeof(t_v3f) * points));
	pack->flags = (unsigned char*)((unsigned long)pack->uv + sizeof(t_v2f) *
		points);
	pack->points = points;
	pack->faces = NULL;
	pack->faces_count = 0;
	return (pack);
}

static t_v3f			load_vertex(const char *line)
{
	char			**tab;
	size_t			size;
	t_v3f			vertex;

	if ((!line) || (!*line))
		return ((t_v3f){0.0f, 0.0f, 0.0f});
	tab = ft_strsplit(line, ' ');
	size = ft_tabcount((void**)tab);
	if (size > 3)
	{
		vertex.x = (float)ft_atod(tab[1]);
		vertex.y = (float)ft_atod(tab[2]);
		vertex.z = (float)ft_atod(tab[3]);
		ft_printf("vertex: %lk\n", &vertex_debug, &vertex);
	}
	else
		vertex = (t_v3f){0.0f, 0.0f, 0.0f};
	ft_freesplit(tab);
	return (vertex);
}

static t_vertex_pack	*load_vertexs(t_list *vlist)
{
	const size_t		size = ft_lstsize(vlist);
	size_t				point;
	t_vertex_pack		*pack;
	t_list				*lst;

	ft_printf("allocating %d points\n", (int)size);
	if (!(pack = makepack(size)))
		return (NULL);
	lst = vlist;
	point = 0;
	while (lst)
	{
		pack->vertex[point] = load_vertex(lst->content);
		pack->uv[point] = (t_v2f){0.0f, 1.0f};
		pack->flags[point] = 0;
		point++;
		lst = lst->next;
	}
	return (pack);
}

static t_vertex_pack	*load_obj_real(const int fd)
{
	t_list			*lst_vertex;
	t_list			*lst_faces;
	char			*line;
	size_t			points;
	t_vertex_pack	*pack;

	lst_vertex = NULL;
	lst_faces = NULL;
	points = 0;
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (!ft_strncmp(line, "vt ", 3))
			;
		else if ((line[0] == 'v') && (++points))
			ft_lstpush_back(&lst_vertex, ft_lstnewlink(line, 0));
		else if (line[0] == 'f')
			ft_lstadd(&lst_faces, ft_lstnewlink(line, 0));
		else
			free(line);
	}
	pack = load_vertexs(lst_vertex);
	pack->faces = load_faces(lst_faces, (int)pack->points, &pack->faces_count);
	ft_lstdel(&lst_vertex, ft_lstpulverisator);
	ft_lstdel(&lst_faces, ft_lstpulverisator);
	return (pack);
}

void					clean_pack(t_vertex_pack *pack)
{
	if (!pack)
		return ;
	free(pack->faces);
	free(pack);
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
