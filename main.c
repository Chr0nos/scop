/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/07/26 15:00:43 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "draw.h"
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
				(t_v4d){off_x, 0.0, 0.0, 0.0});
	glColor3ub((pt->color & 0xff0000) >> 16, (pt->color & 0x00ff00) >> 8,
		pt->color & 0xff);
	glVertex3d(v.x, v.y, v.z);
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
		(t_v4d){0.0, 0.0, 0.0, 1.0});
	return (m);
}

static inline void	init_cube(t_pt_c *cube)
{
	cube[0] = (t_pt_c){1.0f, 1.0f, 1.0f, 0xff0000};
	cube[1] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x0000ff};
	cube[2] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ffff};
	cube[3] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0x00ff00};
	cube[4] = (t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000};
	cube[5] = (t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff};
	cube[6] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x00ffff};
	cube[7] = (t_pt_c){1.0f, 1.0f, 1.0f, 0xb895cb};
	cube[8] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0xffab00};
	cube[9] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x0cc0ff};
	cube[10] = (t_pt_c){1.0f, -1.0f, -1.0f, 0xf0fff0};
	cube[11] = (t_pt_c){1.0f, -1.0f, 1.0f, 0x053916};
	cube[12] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0xffffff};
	cube[13] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x0000ff};
	cube[14] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff};
	cube[15] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0x00ff00};
	cube[16] = (t_pt_c){1.0f, 1.0f, -1.0f, 0x000000 >> 1};
	cube[17] = (t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff >> 1};
	cube[18] = (t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff >> 1};
	cube[19] = (t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ff00};
	cube[20] = (t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000 << 1};
	cube[21] = (t_pt_c){1.0f, 1.0f, 1.0f, 0x0000ff << 1};
	cube[22] = (t_pt_c){-1.0f, 1.0f, 1.0f, 0x00fffa};
	cube[23] = (t_pt_c){-1.0f, -1.0f, 1.0f, 0x341e09};
}

static void			display(void)
{
	unsigned int	p;
	const t_m4		m = make_matrix();
	t_pt_c			pts[POINTS];

	init_cube(pts);
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
