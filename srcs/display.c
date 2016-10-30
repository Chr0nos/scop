/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/30 17:51:32 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"

static t_m4			make_matrix(GLFWwindow *window)
{
	static double	rot = 0;
	static t_v4d	scale = (t_v4d){0.7, 0.7, 0.7, 0.0};
	t_m4			m;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		scale = geo_addv4(scale, (t_v4d){0.01, 0.01, 0.01, 0.0});
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		scale = geo_addv4(scale, (t_v4d){-0.01, -0.01, -0.01, 0.0});
	if (rot > 10000)
		rot = 0.0;
	else
		rot += 0.012;
	m = geo_mk4_rotxyz(
		(t_v4d){-rot, rot * -0.5, 0.0, 0.0},
		scale,
		(t_v4d){0.0, 0.0, 0.0, 1.0});
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

	m = make_matrix(window);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd((GLdouble *)&m);
	//display_pack(pack, texture);
	display_pack_lines(pack);
	(void)display_pack_lines;
	(void)display_pack;
	(void)texture;
}
