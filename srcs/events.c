/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:16:55 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/23 16:08:11 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <GLFW/glfw3.h>

int				keyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return (1);
	return (0);
}

void			framebuffer_size_callback(GLFWwindow *window,
	int width, int height)
{
	t_m4f			proj;

	(void)width;
	(void)height;
	proj = geo_mk4_tof(get_projection(window, DISPLAY_FOV, 1.0, 1000.0));
	glUniformMatrix4fv(get_pack(NULL)->uniforms.proj, 1, GL_FALSE,
		(const float *)&proj);
}

static void		key_press(GLFWwindow *window, t_vertex_pack *pack, int key)
{
	t_uniforms		*u;

	(void)window;
	u = &pack->uniforms;
	if (key == GLFW_KEY_C)
	{
		if ((u->texture_switch_mode == FLAG_SW_NONE) &&
				(u->texture_switch_val < 1.0f))
			u->texture_switch_mode = FLAG_SW_OUT;
		else
			u->texture_switch_mode = FLAG_SW_IN;
	}
}

void			key_callback(GLFWwindow *window, int key, int scancode,
		int action)
{
	(void)scancode;
	if (action == 1)
		key_press(window, get_pack(NULL), key);
	//ft_printf("key pressed: %d - %d -- %d\n", key, scancode, action);
}
