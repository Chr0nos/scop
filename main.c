/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/07/27 21:16:44 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include "ogl.h"
#include "draw.h"
#include <SOIL2.h>
#include <GL/glut.h>
#define POINTS 24

static void			display_vertex(const t_m4 *m, const t_pt_c *pt)
{
	static double	off_x = -0.4;
	static char		sens = 0;
	t_v4d			v;

	if (off_x >= 0.4)
		sens = 1;
	else if (off_x <= -0.4)
		sens = 0;
	off_x += (sens) ? -0.00025 : 0.00025;
	v = geo_addv4(
			geo_apply_m4(
				(t_v4d){(double)pt->x, (double)pt->y, (double)pt->z, 0.0}, m),
				(t_v4d){off_x, cos(off_x * 3.0) * 0.3, 0.0, 0.0});
	if (pt->tx_enabled)
	{
		glColor3ub(0xff, 0xff, 0xff);
		glTexCoord2i((int)pt->tx, (int)pt->ty);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		glColor3ub((pt->color & 0xff0000) >> 16,
			(pt->color & 0x00ff00) >> 8,
			pt->color & 0xff);
	}
	glVertex3d(v.x, v.y, v.z);
}

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

static inline void	init_cube(t_pt_c *cube)
{
	cube[0] = (t_pt_c){1.0f, 1.0f, 1.0f, 0xff0000, 0, 0, 1};
	cube[1] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x0000ff, 0, 1, 1};
	cube[2] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ffff, 1, 0, 1};
	cube[3] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0x00ff00, 1, 1, 1};
	cube[4] = (t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000, 0, 0, 0};
	cube[5] = (t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff, 0, 1, 0};
	cube[6] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x00ffff, 1, 0, 0};
	cube[7] = (t_pt_c){1.0f, 1.0f, 1.0f, 0xb895cb, 1, 1, 0};
	cube[8] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0xffab00, 0, 0, 0};
	cube[9] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x0cc0ff, 0, 0, 0};
	cube[10] = (t_pt_c){1.0f, -1.0f, -1.0f, 0xf0fff0, 0, 0, 0};
	cube[11] = (t_pt_c){1.0f, -1.0f, 1.0f, 0x053916, 0, 0, 0};
	cube[12] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0xffffff, 0, 0, 0};
	cube[13] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x0000ff, 0, 0, 0};
	cube[14] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff, 0, 0, 0};
	cube[15] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0x00ff00, 0, 0, 0};
	cube[16] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x000000 >> 1, 0, 0, 0};
	cube[17] = (t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff >> 1, 0, 0, 0};
	cube[18] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff >> 1, 0, 0, 0};
	cube[19] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ff00, 0, 0, 0};
	cube[20] = (t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000 << 1, 0, 0, 0};
	cube[21] = (t_pt_c){1.0f, 1.0f, 1.0f, 0x0000ff << 1, 0, 0, 0};
	cube[22] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0x00fffa, 0, 0, 0};
	cube[23] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0x341e09, 0, 0, 0};
}

static void			display(void)
{
	unsigned int	p;
	const t_m4		m = make_matrix();
	t_pt_c			pts[POINTS];
	static char		lock = 0;
	static GLuint	texture = 0;

	if (lock == 0)
	{
		texture = SOIL_load_OGL_texture(
			"herbe.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		lock = 1;
	}
	init_cube(pts);
	p = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	while (p < POINTS)
		display_vertex(&m, &pts[p++]);
	glEnd();
	glutSwapBuffers();
	glutPostRedisplay();
}

int					main(int ac, char **av)
{
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Test");
	glutInitWindowPosition(50, 50);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glutDisplayFunc(display);
	glutMainLoop();
	return (0);
}
