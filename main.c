/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/21 23:22:19 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include "ogl.h"
#include "draw.h"
#include "libft.h"
#include <SOIL2.h>
#include <GLFW/glfw3.h>
#define POINTS 24

static void			display_vertex(const t_pt_c *pt)
{
	if (pt->tx_enabled)
	{
		glColor3ub(0xff, 0xff, 0xff);
		glTexCoord2i((int)pt->tx, (int)pt->ty);
	}
	else
		glColor3ub((pt->color & 0xff0000) >> 16,
			(pt->color & 0x00ff00) >> 8, pt->color & 0xff);
	glVertex3d(pt->x, pt->y, pt->z);
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

static void			display(const GLuint texture)
{
	int				p;
	const t_m4		m = make_matrix();
	t_pt_c			pts[POINTS];

	init_cube(pts);
	p = -1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd((GLdouble *)&m);
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

static int			keyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return (1);
	return (0);
}

static void			framebuffer_size_callback(GLFWwindow *window,
	int width, int height)
{
	const float ratio = (float)width / (float)height;

	(void)window;
	ft_printf("window resized: %4dx%-4d ratio: %4f\n", width, height, ratio);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
}

static GLuint		texture_load(const char *filepath)
{
	glEnable(GL_TEXTURE_2D);
	return (SOIL_load_OGL_texture(
		filepath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
}

int					main(int ac, char **av)
{
	GLFWwindow		*window;
	GLuint			texture;

	if (!glfwInit())
		return (1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	if (!(window = glfwCreateWindow(800, 600, "Scope", NULL, NULL)))
	{
		glfwTerminate();
		return (2);
	}
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	texture = texture_load((ac > 1) ? av[1] : "herbe.jpg");
	ft_printf("Renderer: %s\nOpenGL version supported %s\n",
		glGetString(GL_RENDERER), glGetString(GL_VERSION));
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		display(texture);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return (0);
}
