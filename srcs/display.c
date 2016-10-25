/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 21:08:47 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static t_m4			make_matrix(void)
{
	static double	rot = 0;
	t_m4			m;

	if (rot > 10000)
		rot = 0.0;
	else
		rot += 0.012;
	m = geo_mk4_rotxyz(
		(t_v4d){-rot, rot * -0.5, 0.0, 0.0},
		(t_v4d){0.7, 0.7, 0.7, 0.0},
		(t_v4d){0.0, 0.0, 0.0, 1.0});
	return (m);
}

static void			display_pack(t_vertex_pack *pack)
{
	size_t		face;
	t_v3i		*index;

	face = pack->faces_count;
	glDisable(GL_TEXTURE_2D);
	//glBegin(GL_TRIANGLES);
	glBegin(GL_LINES);
	glColor3ub(255, 255, 255);
	while (face--)
	{
		index = &pack->faces[face];
		glVertex3fv((const GLfloat*)&pack->vertex[index->x]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->y]);

		glVertex3fv((const GLfloat*)&pack->vertex[index->z]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->x]);

		glVertex3fv((const GLfloat*)&pack->vertex[index->y]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->z]);
	}
	glEnd();
}

void				display(const GLuint texture, t_vertex_pack *pack)
{
	int				p;
	t_m4			m;

	m = make_matrix();
	p = -1;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd((GLdouble *)&m);
	display_pack(pack);
	(void)texture;
}
