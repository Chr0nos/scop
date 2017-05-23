/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/23 02:04:08 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_quaternion *q,
	t_v4d *cam, const double speed)
{
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		cam->z -= speed;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		cam->z += speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->x -= speed;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->x += speed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->y += speed;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->y -= speed;
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

t_m4				make_matrix(GLFWwindow *window)
{
	static t_v4d			camera = (t_v4d){0.0, 0.0, 5.0, 1.0};
	t_m4					m;
	static t_quaternion		q = (t_quaternion){1.0, 0.0, 0.0, 0.0};

	matrix_keyboard(window, &q, &camera,
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 0.5 : 0.1);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m = geo_quat_tomatrix(q);
	m.w = camera;
	return (m);
}

t_m4				get_projection(GLFWwindow *window, double fov, double far,
	double near)
{
	double			ratio;
	double			height;
	double			width;
	t_m4			proj;
	t_v2i			geo;

	glfwGetWindowSize(window, &geo.x, &geo.y);
	ratio = (double)geo.x / (double)geo.y;
	height = near * (tan(fov * 0.5 * (M_PI / 180)));
	width = height * ratio;
	proj = geo_mk4_projection(
		(t_proj){-width, width, -height, height, near, far});
	geo_putm4(proj, 6);
	return (proj);
}

int					display_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	const int		faces_total = (int)(pack->stats.faces * 3);
	t_m4f			proj;
	t_m4f			modelview;

	proj = geo_mk4_tof(get_projection(window, 45, 1.0, 100.0));
	glUseProgram(pack->program);
	pack->proj_id = glGetUniformLocation(pack->program, "projection");
	pack->model_id = glGetUniformLocation(pack->program, "model");
	glUniformMatrix4fv(pack->proj_id, 1, GL_FALSE, (const GLfloat *)&proj);
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		modelview = geo_mk4_tof(make_matrix(window));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(pack->model_id, 1, GL_FALSE,
			(const GLfloat *)&modelview);
		glBindVertexArray(pack->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->indices);
		glDrawElements(GL_TRIANGLES, faces_total, GL_UNSIGNED_INT, NULL);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return (0);
}
