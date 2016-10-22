/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/22 15:21:47 by snicolet         ###   ########.fr       */
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

static void			display_vertex(const t_pt_c *pt)
{
	if (pt->tx_enabled)
	{
		glColor3ub(0xff, 0xff, 0xff);
		glTexCoord2f(pt->uv.x, pt->uv.y);
	}
	else
		glColor3ub((pt->color & 0xff0000) >> 16,
			(pt->color & 0x00ff00) >> 8, pt->color & 0xff);
	glVertex3d(pt->pos.x, pt->pos.y, pt->pos.z);
}

void				display(const GLuint texture, t_pt_c *pts)
{
	int				p;
	const t_m4		m = make_matrix();

	p = -1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd((GLdouble *)&m);
	glTranslatef(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	while (++p < POINTS)
		if (!(pts[p].tx_enabled))
			display_vertex(&pts[p]);
	glEnd();
	p = -1;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	while (++p < POINTS)
		if (pts[p].tx_enabled)
			display_vertex(&pts[p]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
