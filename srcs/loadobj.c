/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loadobj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 12:56:21 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/13 22:43:09 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>

static void				load_obj_uv(t_list **lst_uv, char *line)
{
	static t_list	*last = NULL;
	t_v2f			uv;

	if (ft_sscanf(line, "vt %f %f", &uv.x, &uv.y) >= 0)
		last = ft_lstpush_back((last) ? &last : lst_uv,
			 ft_lstnew(&uv, sizeof(t_v2f)));
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

static t_vertex_pack	*load_obj_real_pack(t_list *lst_vertex,
	t_list *lst_faces, t_list *lst_uv)
{
	t_vertex_pack	*pack;

	pack = load_vertexs(lst_vertex);
	pack->faces = load_faces(lst_faces, (int)pack->points, &pack->faces_count);
	load_obj_uv_final(lst_uv, pack);
	ft_lstdel(&lst_vertex, ft_lstpulverisator);
	ft_lstdel(&lst_faces, ft_lstpulverisator);
	ft_lstdel(&lst_uv, ft_lstpulverisator);
	return (pack);
}

static t_vertex_pack	*load_obj_real(const int fd)
{
	t_list			*lst_vertex;
	t_list			*lst_faces;
	t_list			*lst_uv;
	char			*line;
	size_t			points;

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
	return (load_obj_real_pack(lst_vertex, lst_faces, lst_uv));
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
