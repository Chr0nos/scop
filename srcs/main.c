/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/04 18:25:58 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static GLuint		texture_load(const char *filepath)
{
	glEnable(GL_TEXTURE_2D);
	return (SOIL_load_OGL_texture(
		filepath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y));
}

void				error_handler(int id, const char *str)
{
	ft_dprintf(2, "[%2d] error: %s\n", id, str);
}

/*
** this function create the indices buffer used to know wich vertices goes
** with wich face
*/

static void			make_indices(t_vertex_pack *pack)
{
	ft_putendl("making faces indices");
	ft_opengl_buffer_load(&pack->indices, GL_ELEMENT_ARRAY_BUFFER,
		pack->faces, pack->stats.faces * 12);
	pack->normal = 0;
	if (pack->stats.normal)
	{
		ft_putendl("making normals indices");
		ft_opengl_buffer_load(&pack->normal, GL_ARRAY_BUFFER, pack->normals,
			pack->stats.normal);
	}
}

static int			make_vao(t_vertex_pack *pack)
{
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)(sizeof(float) * pack->stats.vertex * 3),
		(float*)pack->vertex, GL_STATIC_DRAW);
	make_indices(pack);
	ft_putendl("making vao");
	pack->vao = 0;
	glGenVertexArrays(1, &pack->vao);
	glBindVertexArray(pack->vao);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);
	return (0);
}

static int			make_program(t_vertex_pack *pack)
{
	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, "vertex.glsl")))
		return (2);
	ft_putendl("shaders ok");
	ft_printf("loading texture: %s\n", pack->texture_path);
	pack->texture = texture_load(pack->texture_path);
	glBindTexture(GL_TEXTURE_2D, pack->texture);
	pack->program = glCreateProgram();
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	make_vao(pack);
	ft_putendl("program done");
	return (0);
}

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
	make_program(pack);
	return (display_loop(window, pack));
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

static int			run_parse(const char *filepath, const char *texture)
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

int					main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	if (!glfwInit())
		return (11);
	ft_printf("Init ok\nGlfw version: %s\n", glfwGetVersionString());
	return (run_parse(av[1], (ac > 2) ? av[2] : NULL));
}
