/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/04/28 01:23:10 by snicolet         ###   ########.fr       */
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

t_m4				get_projection(GLFWwindow *window, double fov, double far,
	double near)
{
	double			ratio;
	double			height;
	double			width;
	t_m4			proj;
	t_v2i			geo;

	glfwGetWindowSize(window, &geo.x, &geo.y);
	ratio = (double)geo.x / (double)geo.y;
	height = near * (tan(fov * 0.5 * (M_PI / 180)));
	width = height * ratio;
	proj = geo_mk4_projection(
		(t_proj){-width, width, -height, height, near, far});
	geo_putm4(proj, 6);
	return (proj);
}

static int			main_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	const int		faces_total = (int)(pack->stats.faces * 3);
	t_m4f			proj;
	t_m4f			modelview;

	proj = geo_mk4_tof(get_projection(window, 60, 1.0, 100.0));
	glUseProgram(pack->program);
	pack->proj_id = glGetUniformLocation(pack->program, "projection");
	pack->model_id = glGetUniformLocation(pack->program, "model");
	glUniformMatrix4fv(pack->proj_id, 1, GL_FALSE, (const GLfloat *)&proj);
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		modelview = geo_mk4_tof(make_matrix(window));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(pack->model_id, 1, GL_FALSE,
			(const GLfloat *)&modelview);
		glBindVertexArray(pack->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->indices);
		glDrawElements(GL_TRIANGLES, faces_total,
				GL_UNSIGNED_INT, NULL);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	free(pack);
	return (0);
}

void				error_handler(int id, const char *str)
{
	ft_dprintf(2, "[%2d] error: %s\n", id, str);
}

static void			configure_opengl()
{
	glfwSetErrorCallback(&error_handler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

/*
** this function create the indices buffer used to know wich vertices goes
** with wich face
*/

static void			make_indices(t_vertex_pack *pack)
{
	ft_putendl("making indices");
	pack->indices = 0;
	glGenBuffers(1, &pack->indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack->indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(pack->stats.faces * 12),
		pack->faces, GL_STATIC_DRAW);
}

static int			make_program(t_vertex_pack *pack, const char *texture_path)
{
	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, "vertex.glsl")))
		return (2);
	ft_putendl("shaders ok");
	pack->texture = texture_load(texture_path);
	glBindTexture(GL_TEXTURE_2D, pack->texture);
	pack->program = glCreateProgram();
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(float) * pack->stats.vertex * 3),
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
	ft_putendl("program done");
	return (0);
}

int					main(int ac, char **av)
{
	GLFWwindow		*window;
	t_vertex_pack	*pack;

	if (!glfwInit())
		return (1);
	ft_printf("Init ok\nGlfw version: %s\n", glfwGetVersionString());
	if (!(pack = parse_obj(av[1])))
	{
		glfwTerminate();
		return (2);
	}
	configure_opengl();
	if (!(window = glfwCreateWindow(1280, 720, "Scope", NULL, NULL)))
	{
		glfwTerminate();
		return (3);
	}
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if (glewInit() != GLEW_OK)
		ft_dprintf(2, "HOLY CRAP !\n");
	glClearDepth((double)(INFINITY));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	ft_printf("program state: %d\n",
		make_program(pack, (ac > 2) ? av[2] : "herbe.jpg"));
	ft_printf("Renderer: %s\nOpenGL version supported %s\n",
		glGetString(GL_RENDERER), glGetString(GL_VERSION));
	return (main_loop(window, pack));
}
