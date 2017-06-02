/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:16:55 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/02 17:57:08 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <GLFW/glfw3.h>

static void		update_projection(GLFWwindow *window, t_vertex_pack *pack)
{
	t_m4f	proj;

	proj = geo_mk4_tof(get_projection(window, pack->fov, 1.0, 1000.0));
	glUniformMatrix4fv(pack->uniforms.proj, 1, GL_FALSE, (const float *)&proj);
}

void			framebuffer_size_callback(GLFWwindow *window,
	int width, int height)
{
	(void)width;
	(void)height;
	update_projection(window, get_pack(NULL));
	glViewport(0, 0, width, height);
}

static void		update_fov(GLFWwindow *window, t_vertex_pack *pack,
	const double value)
{
	pack->fov += value;
	if (pack->fov < 20.0)
		pack->fov = 20.0;
	else if (pack->fov > 180.0)
		pack->fov = 180.0;
	update_projection(window, pack);
}

static int		change_polygon_mode(const int key)
{
	if (key == GLFW_KEY_I)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (key == GLFW_KEY_O)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (key == GLFW_KEY_P)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		return (0);
	return (1);
}

static void		key_press(GLFWwindow *window, t_vertex_pack *pack, int key)
{
	t_uniforms		*u;

	u = &pack->uniforms;
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_C)
	{
		if ((u->texture_switch_mode == FLAG_SW_NONE) &&
				(u->texture_switch_val < 1.0f))
			u->texture_switch_mode = FLAG_SW_OUT;
		else
			u->texture_switch_mode = FLAG_SW_IN;
	}
	else if (change_polygon_mode(key))
		;
	else if (key == GLFW_KEY_R)
		reset_camera(pack);
	else if (key == GLFW_KEY_T)
		reset_model(pack);
	else if (key == GLFW_KEY_M)
		update_fov(window, pack, 5.0);
	else if (key == GLFW_KEY_N)
		update_fov(window, pack, -5.0);
}

static void		light_move(int key, t_vertex_pack *pack, const float speed)
{
	t_v3f	*pos;

	pos = &pack->light.position;
	if (key == GLFW_KEY_KP_8)
		pos->y += speed;
	else if (key == GLFW_KEY_KP_5)
		pos->y += -speed;
	else if (key == GLFW_KEY_KP_4)
		pos->x += speed;
	else if (key == GLFW_KEY_KP_6)
		pos->x += -speed;
	else if (key == GLFW_KEY_KP_7)
		pos->z += -speed;
	else if (key == GLFW_KEY_KP_9)
		pos->z += speed;
	else
		return ;
	ft_printf("new light position: %.1f %.1f %.1f\n",
			(double)pos->x, (double)pos->y, (double)pos->z);
	glUniform3fv(pack->uniforms.light_pos, 1,
		(const GLfloat*)&pack->light.position);
}

void			key_callback(GLFWwindow *window, int key, int scancode,
		int action)
{
	(void)scancode;
	if (action == 1)
		key_press(window, get_pack(NULL), key);
	light_move(key, get_pack(NULL), 0.5f);
}
