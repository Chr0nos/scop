/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_vertex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/10 17:05:57 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/10 17:06:14 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"

t_vertex_pack			*load_vertexs(t_list *vlist)
{
	const size_t		size = ft_lstsize(vlist);
	size_t				point;
	t_vertex_pack		*pack;
	t_list				*lst;
	t_v3f				*v;

	ft_printf("allocating %d points\n", (int)size);
	if (!(pack = makepack(size)))
		return (NULL);
	lst = vlist;
	point = 0;
	while (lst)
	{
		v = &pack->vertex[point];
		*v = (t_v3f){0.0f, 0.0f, 0.0f};
		ft_sscanf(lst->content, "v %f\\s%f\\s%f", &v->x, &v->y, &v->z);
		pack->uv[point] = (t_v2f){0.0f, 1.0f};
		pack->flags[point] = 0;
		point++;
		lst = lst->next;
	}
	return (pack);
}

