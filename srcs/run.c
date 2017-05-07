/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:33:10 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/07 18:48:21 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static void			configure_opengl(void)
{
	glfwSetErrorCallback(&error_handler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

static int			run_program(t_vertex_pack *pack, GLFWwindow *window)
{
	int		ret;

	if (make_program(pack) != 0)
	{
		ft_dprintf(2, "error while making program\n");
		return (41);
	}
	ret = display_loop(window, pack);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	return (ret);
}

static int			run_window(t_vertex_pack *pack)
{
	GLFWwindow		*window;
	int				ret;

	ft_putendl("run window");
	configure_opengl();
	window = glfwCreateWindow(1280, 720, "Scope", NULL, NULL);
	ret = 3;
	if (window)
	{
		glfwMakeContextCurrent(window);
		ft_printf("Renderer: %s\nOpenGL version supported %s\n",
		glGetString(GL_RENDERER), glGetString(GL_VERSION));
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		if (glewInit() != GLEW_OK)
			ret = 31;
		else
		{
			glClearDepth((double)(INFINITY));
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			ret = run_program(pack, window);
		}
		ft_putendl("deleting window");
		glfwDestroyWindow(window);
	}
	return (ret);
}

int				run_parse(const char *filepath, const char *texture)
{
	int				ret;
	t_vertex_pack	*pack;

	ft_putendl("run parse");
	pack = parse_obj(filepath);
	ret = 2;
	if (pack)
	{
		pack->texture_path = (texture) ? texture : "herbe.jpg";
		ret = run_window(pack);
		ft_putendl("cleaning main structure pack");
		free(pack);
	}
	ft_putendl("cleaning glfw");
	glfwTerminate();
	ft_printf("final return code: %d\n", ret);
	return (ret);
}
