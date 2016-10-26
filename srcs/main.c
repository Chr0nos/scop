/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/25 23:08:30 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"

static GLuint		texture_load(const char *filepath)
{
	glEnable(GL_TEXTURE_2D);
	return (SOIL_load_OGL_texture(
		filepath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
}

static int			main_loop(GLFWwindow *window, GLuint texture,
	const char *filepath)
{
	t_vertex_pack	*pack;

	if (!(pack = load_obj(filepath)))
	{
		glfwTerminate();
		return (-1);
	}
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(texture, pack, window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	clean_pack(pack);
	return (0);
}

void				error_handler(int id, const char *str)
{
	ft_printf("[%2d] error: %s\n", id, str);
}

int					main(int ac, char **av)
{
	GLFWwindow		*window;
	GLuint			texture;

	if (!glfwInit())
		return (1);
	ft_printf("Init ok\nGlfw version: %s\n", glfwGetVersionString());
	glfwSetErrorCallback(&error_handler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	if (!(window = glfwCreateWindow(1280, 720, "Scope", NULL, NULL)))
	{
		glfwTerminate();
		return (2);
	}
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	texture = texture_load((ac > 2) ? av[2] : "herbe.jpg");
	ft_printf("Renderer: %s\nOpenGL version supported %s\n",
		glGetString(GL_RENDERER), glGetString(GL_VERSION));
	return (main_loop(window, texture, av[1]));
}
