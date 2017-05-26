/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixcenter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 16:55:22 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 14:50:50 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static void				load_min(t_v3f a, t_v3f *b)
{
	if (a.x < b->x)
		b->x = a.x;
	if (a.y < b->y)
		b->y = a.y;
	if (a.z < b->z)
		b->z = a.z;
}

static void				load_max(t_v3f a, t_v3f *b)
{
	if (a.x > b->x)
		b->x = a.x;
	if (a.y > b->y)
		b->y = a.y;
	if (a.z > b->z)
		b->z = a.z;
}

static t_v3f			get_center(t_vertex_pack *pack, size_t n)
{
	t_vertex_item	*i;
	t_v3f			min;
	t_v3f			max;

	min = pack->items[0].position;
	max = pack->items[0].position;
	while (n--)
	{
		i = &pack->items[n];
		load_min(i->position, &min);
		load_max(i->position, &max);
	}
	return ((t_v3f){
		.x = min.x + (max.x - min.x) * 0.5f,
		.y = min.y + (max.y - min.y) * 0.5f,
		.z = min.z + (max.z - min.z) * 0.5f
	});
}

void					fixcenter(t_vertex_pack *pack)
{
	size_t		p;

	pack->center = get_center(pack, pack->stats.vertex);
	p = pack->stats.vertex;
	while (p--)
		pack->items[p].position =
			geo_subv3(pack->items[p].position, pack->center);
}
