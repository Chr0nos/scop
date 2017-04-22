/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:18:49 by snicolet          #+#    #+#             */
/*   Updated: 2017/04/22 19:58:43 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "geo.h"
#include "quaternion.h"

static void			matrix_keyboard(GLFWwindow *window, t_quaternion *q,
	t_v4d *cam)
{
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		cam->z -= 0.1;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		cam->z += 0.1;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->x -= 0.1;
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->x += 0.1;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->y += 0.1;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->y -= 0.1;
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

	matrix_keyboard(window, &q, &camera);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m = geo_quat_tomatrix(q);
	m.w = camera;
	return (m);
}
