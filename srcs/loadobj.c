/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/01 16:43:07 by snicolet         ###   ########.fr       */
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
	ft_bzero(pack->flags, points);
	pack->points = points;
	pack->faces = NULL;
	pack->faces_count = 0;
	return (pack);
}

static t_v3f			load_vertex(const char *line)
{
	t_v3f			vertex;

	vertex = (t_v3f){0.0f, 0.0f, 0.0f};
	ft_sscanf(line, "v %f\\s%f\\s%f", &vertex.x, &vertex.y, &vertex.z);
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

static void				load_obj_uv(t_list **lst_uv, char *line)
{
	t_v2f		uv;

	if (ft_sscanf(line, "vt %f %f", &uv.x, &uv.y) >= 0)
		ft_lstpush_back(lst_uv, ft_lstnew(&uv, sizeof(t_v2f)));
}

static void				load_obj_uv_final(t_list *lst_uv, t_vertex_pack *pack)
{
	size_t		p;

	p = 0;
	while ((lst_uv) && (p < pack->points))
	{
		pack->uv[p] = *(t_v2f*)lst_uv->content;
		pack->flags[p] |= FLAG_UV;
		p++;
		lst_uv = lst_uv->next;
	}
}


static t_vertex_pack	*load_obj_real(const int fd)
{
	t_list			*lst_vertex;
	t_list			*lst_faces;
	t_list			*lst_uv;
	char			*line;
	size_t			points;
	t_vertex_pack	*pack;

	lst_vertex = NULL;
	lst_faces = NULL;
	lst_uv = NULL;
	points = 0;
	while (ft_get_next_line(fd, &line) > 0)
	{
		if (!ft_strncmp(line, "vt ", 3))
			load_obj_uv(&lst_uv, line);
		else if ((line[0] == 'v') && (++points))
			ft_lstpush_back(&lst_vertex, ft_lstnewlink(line, 0));
		else if (line[0] == 'f')
			ft_lstadd(&lst_faces, ft_lstnewlink(line, 0));
		else
			free(line);
	}
	free(line);
	pack = load_vertexs(lst_vertex);
	pack->faces = load_faces(lst_faces, (int)pack->points, &pack->faces_count);
	load_obj_uv_final(lst_uv, pack);
	ft_lstdel(&lst_vertex, ft_lstpulverisator);
	ft_lstdel(&lst_faces, ft_lstpulverisator);
	ft_lstdel(&lst_uv, ft_lstpulverisator);
	return (pack);
}

void					clean_pack(t_vertex_pack *pack)
{
	if (!pack)
		return ;
	free(pack->faces);
	free(pack);
}

static void				fixcenter(t_vertex_pack *pack)
{
	size_t		p;

	p = pack->points;
	while (p--)
		pack->vertex[p] = geo_subv3(pack->vertex[p], pack->center);
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
	pack->center = geo_center_v3(pack->vertex, pack->points);
	fixcenter(pack);
	return (pack);
}
