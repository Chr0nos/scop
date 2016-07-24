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
	const t_v4d		v = geo_apply_m4(
		(t_v4d){
			(double)pt->x,
			(double)pt->y,
			(double)pt->z,
			0.0},
		m);

	(void)v;
	gl_setrgb(pt->color);
	glVertex3d(v.x, v.y, v.z);
	//glVertex3d((double)pt->x, (double)pt->y, (double)pt->z);
}

static t_m4			make_matrix(void)
{
	static double	rot = 0;

	if (rot > 10000)
		rot = 0.0;
	else
		rot += 0.012;
	return (geo_mk4_rotxyz(
		(t_v4d){-rot, rot / -2.0, 0.0, 0.0},
		(t_v4d){0.7, 0.7, 0.7, 0.7},
		(t_v4d){0.0, 0.0, 0.0, 0.0}));
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
		(t_pt_c){1.0f, 1.0f, 1.0f, 0x00ff00},

		(t_pt_c){-1.0f, -1.0f, 1.0f, 0xff0000},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x0000ff},
		(t_pt_c){1.0f, 1.0f, -1.0f, 0x00ffff},
		(t_pt_c){1.0f, 1.0f, 1.0f, 0x00ff00},

		(t_pt_c){-1.0f, 1.0f, 1.0f, 0xffffff},
		(t_pt_c){-1.0f, 1.0f, -1.0f, 0x0000ff},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff},
		(t_pt_c){-1.0f, -1.0f, 1.0f, 0x00ff00},

		(t_pt_c){1.0f, 1.0f, -1.0f, 0x000000 >> 1},
		(t_pt_c){1.0f, -1.0f, -1.0f, 0x0000ff >> 1},
		(t_pt_c){-1.0f, -1.0f, -1.0f, 0x00ffff >> 1},
		(t_pt_c){-1.0f, 1.0f, -1.0f, 0x00ff00 >> 1},

		(t_pt_c){1.0f, -1.0f, 1.0f, 0xff0000 << 1},
		(t_pt_c){1.0f, 1.0f, 1.0f, 0x0000ff << 1},
		(t_pt_c){-1.0f, 1.0f, 1.0f, 0x00ffff << 1},
		(t_pt_c){-1.0f, -1.0f, 1.0f, 0x341e09}
	};
	unsigned int	p;

	p = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glRotatef(1.0, 0.01f, 0.0, 0.0);
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
	glutCreateWindow("OpenGL Setup Test");
	glutInitWindowPosition(50, 50);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutMainLoop();
	return (0);
}
