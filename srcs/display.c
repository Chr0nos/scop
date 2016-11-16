/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2016/11/16 16:59:47 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_quaternion *q,
	t_v4d *cam)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->z += 0.1;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->z -= 0.1;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->x += 0.1;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->x -= 0.1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, -0.02));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.02));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, 0.02));
}

static t_m4			make_matrix(GLFWwindow *window)
{
	static t_v4d			camera = (t_v4d){0.0, 0.0, -15.0, 1.0};
	t_m4					m;
	static t_quaternion		q = (t_quaternion){1.0, 0.0, 0.0, 0.0};

	matrix_keyboard(window, &q, &camera);
	m = geo_quat_tomatrix(q);
	m.w = camera;
	return (m);
}

static void			display_pack_lines(t_vertex_pack *pack)
{
	size_t		face;
	t_v3i		*index;

	face = pack->stats.faces;
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

	face = pack->stats.faces;
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
