/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_postprocess.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 15:21:46 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/06 09:21:06 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

int						parse_post_process(t_vertex_pack *pack)
{
	const int			mv = (int)pack->stats.vertex - 1;
	size_t				p;

	p = pack->stats.faces;
	while (p--)
	{
		if ((pack->faces[p].x > mv) || (pack->faces[p].y > mv) ||
				(pack->faces[p].z > mv))
		{
			ft_printf("%lu -> %d %d %d\n", p, pack->faces[p].x,
					pack->faces[p].y, pack->faces[p].z);
			ft_dprintf(2, "error: invalid face in obj file !\n");
			return (1);
		}
	}
	fixcenter(pack);
	parse_duplicate(pack);
	return (0);
}
