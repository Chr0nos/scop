/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:16:55 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 22:59:40 by snicolet         ###   ########.fr       */
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
	const double	ratio = (double)width / (double)height;
	const GLdouble	top = (tan(PI180) * 70 * 0.5);
	const GLdouble	bottom = -top;
	const GLdouble	right = top * ratio;
	const GLdouble	left = -right;

	(void)window;
	ft_printf("window resized: %4dx%-4d ratio: %4f "
		"{left: %f right: %f bottom: %f top: %f}\n",
		width, height, ratio, top, bottom, left, right);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.0, 1.0, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
