/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/14 20:26:48 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_v4d *scale,
	t_quaternion *q)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		*scale = geo_addv4(*scale, (t_v4d){0.01, 0.01, 0.01, 0.0});
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		*scale = geo_addv4(*scale, (t_v4d){-0.01, -0.01, -0.01, 0.0});
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, -0.02));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.02));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.02));
}

static t_m4			make_matrix(GLFWwindow *window)
{
	static double			rot = 0;
	static t_v4d			scale = (t_v4d){0.7, 0.7, 0.7, 0.0};
	t_m4					m;
	static t_quaternion		q = (t_quaternion){1.0, 0.0, 0.0, 0.0};

	//q = geo_quat_mult(q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.02));
	//q = geo_quat_mult(q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.01));
	matrix_keyboard(window, &scale, &q);
	if (rot > 10000)
		rot = 0.0;
	else
		rot += 0.012;


	m = geo_mk4_rotxyz(
		(t_v4d){0.0, 0.0, 0.0, 0.0},
		scale,
		(t_v4d){0.0, 0.0, 0.0, 1.0});
	m = geo_mult_m4(geo_quat_tomatrix(q), &m);
	m.w.z -= 15.0;
	return (m);
}

static void			display_pack_lines(t_vertex_pack *pack)
{
	size_t		face;
	t_v3i		*index;

	face = pack->faces_count;
	glDisable(GL_TEXTURE_2D);
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

static void			display_pack(t_vertex_pack *pack, GLuint texture)
{
	size_t		face;
	t_v3i		*index;

	face = pack->faces_count;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	while (face--)
	{
		index = &pack->faces[face];
		if (pack->flags[index->x] & FLAG_UV)
			glTexCoord2fv((const GLfloat*)&pack->uv[index->x]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->x]);
		if (pack->flags[index->y] & FLAG_UV)
			glTexCoord2fv((const GLfloat*)&pack->uv[index->y]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->y]);
		if (pack->flags[index->z] & FLAG_UV)
			glTexCoord2fv((const GLfloat*)&pack->uv[index->z]);
		glVertex3fv((const GLfloat*)&pack->vertex[index->z]);
	}
	glEnd();
}

void				display(const GLuint texture, t_vertex_pack *pack,
	GLFWwindow *window)
{
	t_m4			m;
	static char		mode = 0;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		mode = 1;
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		mode = 0;
	m = make_matrix(window);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd((GLdouble *)&m);
	if (!mode)
		display_pack(pack, texture);
	else
		display_pack_lines(pack);
}
