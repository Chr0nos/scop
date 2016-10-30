/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:16:55 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/30 15:05:15 by snicolet         ###   ########.fr       */
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
	(void)window;
	load_projection((double)width / (double)height, 75, 1.0, 1000.0);
}
