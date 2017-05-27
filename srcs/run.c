/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:33:10 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/28 01:10:12 by snicolet         ###   ########.fr       */
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
	glfwWindowHint(GLFW_SAMPLES, 16);
}

static void			delete_textures(t_vertex_pack *pack)
{
	const GLuint	ids[2] = {
		pack->texture,
		pack->normal_map
	};

	ft_putstr("deleting textures\n");
	glDeleteTextures(2, ids);
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
	delete_textures(pack);
	ft_putendl("cleaning shaders");
	glDeleteShader(pack->fs);
	glDeleteShader(pack->vs);
	ft_putendl("cleaning glProgram");
	glDeleteProgram(pack->program);
	return (ret);
}

/*
** here the callback function for keyboard is casted in something nasty but
** i had no others choices: the coding rules of 42 school are strict on this
** point: no more than 4 parameters for a function
*/

static int			run_window_real(t_vertex_pack *pack, GLFWwindow *window)
{
	glfwSetKeyCallback(window,
			(void(*)(GLFWwindow *, int, int, int, int))(size_t)&key_callback);
	glClearDepth((double)(INFINITY));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return (run_program(pack, window));
}

static int			run_window(t_vertex_pack *pack)
{
	GLFWwindow		*window;
	int				ret;

	ft_putendl("run window");
	configure_opengl();
	window = glfwCreateWindow(1280, 720, "Scop", NULL, NULL);
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
			ret = run_window_real(pack, window);
		ft_putendl("deleting window");
		glfwDestroyWindow(window);
	}
	return (ret);
}

int					run_parse(const char *filepath, const char *texture,
		const char *normal_map)
{
	int				ret;
	t_vertex_pack	pack;

	ft_putendl("run parse");
	ft_bzero(&pack, sizeof(t_vertex_pack));
	get_pack(&pack);
	if (parse_obj(&pack, filepath) != 0)
		return (21);
	ret = 2;
	if (pack.items)
	{
		pack.texture_path = (texture) ? texture : "textures/default.jpg";
		pack.normal_map_path = normal_map;
		ret = run_window(&pack);
		ft_putendl("cleaning main structure pack");
		ft_mfree(2, pack.items, pack.faces);
	}
	ft_putendl("cleaning glfw");
	glfwTerminate();
	ft_printf("final return code: %d\n", ret);
	return (ret);
}
