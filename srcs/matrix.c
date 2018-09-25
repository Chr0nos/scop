/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 14:24:41 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 01:06:50 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

void				auto_rotate(t_vertex_pack *pack)
{
	const double			speed = 0.01;

	if (pack->input & INPUT_AUTOROT)
		pack->model_quat = geo_quat_mult(pack->model_quat,
			geo_quat_rot(AXIS_Y, speed));
}

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
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Y, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Y, -0.02));
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_X, -0.02));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_X, 0.02));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Z, 0.02));
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		*q = geo_quat_mult(*q, geo_quat_rot(AXIS_Z, -0.02));
}

t_m4				make_matrix(GLFWwindow *window, t_vertex_pack *pack)
{
	t_m4					m;

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
		matrix_keyboard(window, &pack->model_quat, &pack->camera.w,
			glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 0.5 : 0.1);
	m = geo_quat_tomatrix(pack->model_quat);
	return (m);
}

void				matrix_init(t_vertex_pack *pack)
{
	pack->flags_shader |= FLAG_SH_NLIGHT;
	if (pack->tex_normal_map.filepath)
		pack->flags_shader |= FLAG_SH_NMAP;
	pack->fov = (double)DISPLAY_FOV;
	pack->camera_quat = geo_quat_identity();
	pack->model_quat = geo_quat_identity();
	pack->model_axis = AXIS_Y;
	pack->camera = geo_quat_tomatrix(pack->camera_quat);
	pack->camera.w = (t_v4d){0.0, 0.0, -8.0, 1.0};
	pack->input |= INPUT_RLAST | INPUT_AUTOROT;
	geo_putm4(pack->camera, 6);
}
