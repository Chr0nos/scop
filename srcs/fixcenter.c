/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixcenter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 16:55:22 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 03:51:28 by snicolet         ###   ########.fr       */
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

static t_v3f			get_center(struct s_object *object, size_t n)
{
	t_vertex_item	*i;
	t_v3f			min;
	t_v3f			max;

	min = object->items[0].position;
	max = object->items[0].position;
	while (n--)
	{
		i = &object->items[n];
		load_min(i->position, &min);
		load_max(i->position, &max);
	}
	return ((t_v3f){
		.x = min.x + (max.x - min.x) * 0.5f,
		.y = min.y + (max.y - min.y) * 0.5f,
		.z = min.z + (max.z - min.z) * 0.5f
	});
}

void					fixcenter(struct s_object *object)
{
	size_t		p;

	object->center = get_center(object, object->stats.vertex);
	p = object->stats.vertex;
	while (p--)
		object->items[p].position =
			geo_subv3(object->items[p].position, object->center);
}
