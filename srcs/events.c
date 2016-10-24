/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:16:55 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/24 15:13:00 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include <GLFW/glfw3.h>
#define PI180 0.017453292519943295769236907684886

int				keyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return (1);
	return (0);
}

void			framebuffer_size_callback(GLFWwindow *window,
	int width, int height)
{
	const float		ratio = (float)width / (float)height;
	const GLdouble	top = (tan(PI180) * 70 * 0.5);
	const GLdouble	bottom = -top;
	const GLdouble	right = top * ratio;
	const GLdouble	left = -right;

	(void)window;
	ft_printf("window resized: %4dx%-4d ratio: %4f {left: %f right: %f bottom: %f top: %f}\n",
		width, height, ratio, top, bottom, left, right);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
	glOrtho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
//	glFrustum(-width, width, -height, height, -1.0, 1.0);
	// glFrustum(left, right, bottom, top, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
