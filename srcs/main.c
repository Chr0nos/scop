/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2016/10/22 18:25:38 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include "ogl.h"

static inline void	init_cube(t_pt_c *cube)
{
	cube[0] = (t_pt_c){(t_v3f){1.0f, 1.0f, 1.0f}, 0xff0000, (t_v2f){0, 0}, 1};
	cube[1] = (t_pt_c){(t_v3f){1.0f, 1.0f, -1.0f}, 0x0000ff, (t_v2f){0, 1}, 1};
	cube[2] = (t_pt_c){(t_v3f){-1.0f, 1.0f, -1.0f}, 0x00ffff, (t_v2f){1, 0}, 1};
	cube[3] = (t_pt_c){(t_v3f){-1.0f, 1.0f, 1.0f}, 0x00ff00, (t_v2f){1, 1}, 1};
	cube[4] = (t_pt_c){(t_v3f){1.0f, -1.0f, 1.0f}, 0xff0000, (t_v2f){0, 0}, 0};
	cube[5] = (t_pt_c){(t_v3f){1.0f, -1.0f, -1.0f}, 0x0000ff, (t_v2f){0, 1}, 0};
	cube[6] = (t_pt_c){(t_v3f){1.0f, 1.0f, -1.0f}, 0x00ffff, (t_v2f){1, 0}, 0};
	cube[7] = (t_pt_c){(t_v3f){1.0f, 1.0f, 1.0f}, 0xb895cb, (t_v2f){1, 1}, 0};
	cube[8] = (t_pt_c){(t_v3f){-1.0f, -1.0f, 1.0f}, 0xffab00, (t_v2f){0, 0}, 0};
	cube[9] = (t_pt_c){(t_v3f){-1.0f, -1.0f, -1.0f}, 0x0cc0ff, (t_v2f){0, 0}, 0};
	cube[10] = (t_pt_c){(t_v3f){1.0f, -1.0f, -1.0f}, 0xf0fff0, (t_v2f){0, 0}, 0};
	cube[11] = (t_pt_c){(t_v3f){1.0f, -1.0f, 1.0f}, 0x053916, (t_v2f){0, 0}, 0};
	cube[12] = (t_pt_c){(t_v3f){-1.0f, 1.0f, 1.0f}, 0xffffff, (t_v2f){0, 0}, 0};
	cube[13] = (t_pt_c){(t_v3f){-1.0f, 1.0f, -1.0f}, 0x0000ff, (t_v2f){0, 0}, 0};
	cube[14] = (t_pt_c){(t_v3f){-1.0f, -1.0f, -1.0f}, 0x00ffff, (t_v2f){0, 0}, 0};
	cube[15] = (t_pt_c){(t_v3f){-1.0f, -1.0f, 1.0f}, 0x00ff00, (t_v2f){0, 0}, 0};
	cube[16] = (t_pt_c){(t_v3f){1.0f, 1.0f, -1.0f}, 0x000000 >> 1, (t_v2f){0, 0}, 0};
	cube[17] = (t_pt_c){(t_v3f){1.0f, -1.0f, -1.0f}, 0x0000ff >> 1, (t_v2f){0, 0}, 0};
	cube[18] = (t_pt_c){(t_v3f){-1.0f, -1.0f, -1.0f}, 0x00ffff >> 1, (t_v2f){0, 0}, 0};
	cube[19] = (t_pt_c){(t_v3f){-1.0f, 1.0f, -1.0f}, 0x00ff00, (t_v2f){0, 0}, 0};
	cube[20] = (t_pt_c){(t_v3f){1.0f, -1.0f, 1.0f}, 0xff0000 << 1, (t_v2f){0, 0}, 0};
	cube[21] = (t_pt_c){(t_v3f){1.0f, 1.0f, 1.0f}, 0x0000ff << 1, (t_v2f){0, 0}, 0};
	cube[22] = (t_pt_c){(t_v3f){-1.0f, 1.0f, 1.0f}, 0x00fffa, (t_v2f){0, 0}, 0};
	cube[23] = (t_pt_c){(t_v3f){-1.0f, -1.0f, 1.0f}, 0x341e09, (t_v2f){0, 0}, 0};
}

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
	t_pt_c			pts[POINTS];

	load_obj(filepath);
	init_cube(pts);
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		(void)texture;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(texture, pts);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
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
	if (!(window = glfwCreateWindow(800, 600, "Scope", NULL, NULL)))
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
