/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/07/25 17:41:21 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "draw.h"
#include <GL/glut.h>
#define POINTS 24

static inline void	gl_setrgb(unsigned int color)
{
	float	c[3];

	c[0] = (float)((color & 0xff0000) >> 16) * 0.003921f;
	c[1] = (float)((color & 0x00ff00) >> 8) * 0.003921f;
	c[2] = (float)((color & 0x0000ff)) * 0.003921f;
	glColor3f(c[0], c[1], c[2]);
}

static void			display_vertex(const t_m4 *m, const t_pt_c *pt)
{
	static double	off_x = -0.4;
	static char		sens = 0;
	const t_v4d		v = geo_apply_m4(
		(t_v4d){
			(double)pt->x,
			(double)pt->y,
			(double)pt->z,
			0.0},
		m);

	if (off_x >= 0.4)
		sens = 1;
	else if (off_x <= -0.4)
		sens = 0;
	off_x += (sens) ? -0.00015 : 0.00015;
	gl_setrgb(pt->color);
	glVertex3d(v.x + off_x, v.y, v.z);
}

static t_m4			make_matrix(void)
{
	static double	rot = 0;
	t_m4			m;
	double			ratio;

	if (rot > 10000)
		rot = 0.0;
	else
		rot += 0.012;
	ratio = (double)glutGet(GLUT_WINDOW_WIDTH) /
		(double)glutGet(GLUT_WINDOW_HEIGHT);
	if (ratio > 1.0)
		ratio = 1.0;
	m = geo_mk4_rotxyz(
		(t_v4d){-rot, rot * -0.5, 0.0, 0.0},
		(t_v4d){0.7, 0.7, 0.7, 0.0},
		(t_v4d){0.0, 0.0, 0.0, 0.0});
	return (m);
}

static void			display(void)
{
	const t_m4		m = make_matrix();
	const t_pt_c	pts[POINTS] = {
		(t_pt_c){1.0f, 1.0f, 1.0f, 0xff0000},
		(t_pt_c){1.0f, 1.0f, -1.0f, 0x0000ff},
		(t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ffff},
		(t_pt_c){-1.0f, 1.0f, 1.0f, 0x00ff00},

		(t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000},
		(t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff},
		(t_pt_c){1.0f, 1.0f, -1.0f, 0x00ffff},
		(t_pt_c){1.0f, 1.0f, 1.0f, 0xb895cb},

		(t_pt_c){-1.0f, -1.0f, 1.0f, 0xffab00},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x0cc0ff},
		(t_pt_c){1.0f, -1.0f, -1.0f, 0xf0fff0},
		(t_pt_c){1.0f, -1.0f, 1.0f, 0x053916},

		(t_pt_c){-1.0f, 1.0f, 1.0f, 0xffffff},
		(t_pt_c){-1.0f, 1.0f, -1.0f, 0x0000ff},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff},
		(t_pt_c){-1.0f, -1.0f, 1.0f, 0x00ff00},

		(t_pt_c){1.0f, 1.0f, -1.0f, 0x000000 >> 1},
		(t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff >> 1},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff >> 1},
		(t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ff00},

		(t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000 << 1},
		(t_pt_c){1.0f, 1.0f, 1.0f, 0x0000ff << 1},
		(t_pt_c){-1.0f, 1.0f, 1.0f, 0x00fffa},
		(t_pt_c){-1.0f, -1.0f, 1.0f, 0x341e09}
	};
	unsigned int	p;

	p = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glutDisplayFunc(display);
	glutMainLoop();
	return (0);
}
