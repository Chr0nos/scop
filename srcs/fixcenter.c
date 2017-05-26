/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixcenter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/16 16:55:22 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/26 13:25:37 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void					fixcenter(t_vertex_pack *pack)
{
	size_t		p;

	p = pack->stats.vertex;
	while (p--)
		pack->items[p].position =
			geo_subv3(pack->items[p].position, pack->center);
}
