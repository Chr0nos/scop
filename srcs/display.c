/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/03 14:13:01 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_quaternion *q,
	t_v4d *cam, const double speed)
{
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		cam->z += speed;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		cam->z -= speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->x += speed;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->x -= speed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->y -= speed;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->y += speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 1.0, 0.0}, -0.02));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){1.0, 0.0, 0.0}, 0.02));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot((t_v3d){0.0, 0.0, 1.0}, -0.02));
}

t_m4				make_matrix(GLFWwindow *window, t_vertex_pack *pack)
{
	t_m4					m;

	matrix_keyboard(window, &pack->model_quat, &pack->camera.w,
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 0.5 : 0.1);
	m = geo_quat_tomatrix(pack->model_quat);
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
	return (proj);
}

/*
** this function manage the folowings keys: Z X C
** press Z to incrase texture intenssity
** press X to decrase texture intenssity
** press C to switch texture intenssity using a smooth fade effect
** the value of u->texture_switch_val will be sent to the graphic card
** it will be already clamped
** the default state of u->texture_switch_mode is FLAG_SW_NONE
*/

static void			event_texture_mode(GLFWwindow *window, t_uniforms *u)
{
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		u->texture_switch_val -= 0.05f;
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		u->texture_switch_val += 0.05f;
	if (u->texture_switch_mode != FLAG_SW_NONE)
	{
		u->texture_switch_val +=
			(u->texture_switch_mode & FLAG_SW_IN) ? -0.015f : 0.015f;
		if ((u->texture_switch_val <= 0.0f) || (u->texture_switch_val >= 1.0f))
			u->texture_switch_mode = FLAG_SW_NONE;
	}
	u->texture_switch_val = geo_clamp(u->texture_switch_val, 0.0f, 1.0f);
	glUniform1f(u->texture_switch, u->texture_switch_val);
}

static void			matrix_init(t_vertex_pack *pack)
{
	if (pack->normal_map_path)
		pack->flags_shader |= FLAG_SH_NMAP;
	pack->fov = (double)DISPLAY_FOV;
	pack->camera_quat = geo_quat_identity();
	pack->model_quat = geo_quat_identity();
	pack->camera = geo_quat_tomatrix(pack->camera_quat);
	pack->camera.w = (t_v4d){0.0, 0.0, -8.0, 1.0};
	pack->input |= INPUT_RLAST;
	geo_putm4(pack->camera, 6);
}

/*
** main display_loop
*/

int					display_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	const int		faces_total = (int)(pack->stats.faces * 3);
	t_m4f			model;
	t_m4f			view;

	matrix_init(pack);
	glUseProgram(pack->program);
	send_uniforms(window, pack);
	while (!glfwWindowShouldClose(window))
	{
		event_texture_mode(window, &pack->uniforms);
		pack->model = make_matrix(window, pack);
		model = geo_mk4_tof(pack->model);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(pack->uniforms.model_view, 1, GL_FALSE,
			(const GLfloat *)&model);
		view = geo_mk4_tof(pack->camera);
		glUniformMatrix4fv(pack->uniforms.camera, 1, GL_FALSE,
			(const GLfloat *)&view);
		glBindVertexArray(pack->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->indices);
		glDrawElements(GL_TRIANGLES, faces_total, GL_UNSIGNED_INT, NULL);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return (0);
}
