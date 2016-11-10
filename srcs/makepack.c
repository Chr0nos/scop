/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makepack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/10 17:07:00 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/10 17:07:46 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "libft.h"

t_vertex_pack			*makepack(const size_t points)
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

