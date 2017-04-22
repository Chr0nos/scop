/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/25 17:36:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/04/22 15:15:54 by snicolet         ###   ########.fr       */
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

t_m4				get_projection(double ratio, double fov, double far,
	double near)
{
	const double	height = near * (tan(fov * 0.5 * (M_PI / 180)));
	const double	width = height * ratio;
	t_m4			proj;

	proj = geo_mk4_projection(
		(t_proj){-width, width, -height, height, near, far});
	geo_putm4(proj, 6);
	return (proj);
}

static int			main_loop(GLFWwindow *window, t_vertex_pack *pack)
{
	t_v2i			geo;
	t_m4			proj;
	t_m4			modelview;

	ft_printf("done\n");
	glfwGetWindowSize(window, &geo.x, &geo.y);
	ft_putendl("loading projection matrix\n");
	//proj = get_projection((double)geo.x / (double)geo.y, 75, 1.0, 100.0);
	proj = geo_mk4_identity();
	modelview = proj;
//	modelview = make_matrix(window);
	geo_putm4(proj, 6);
	geo_putm4(modelview, 6);
	//glDisable(GL_SMOOTH);
	ft_putstr("entering display loop\n");
	pack->proj_id = glGetUniformLocation(pack->program, "projection");
	pack->model_id = glGetUniformLocation(pack->program, "model");
	glUniformMatrix4dv(pack->proj_id, 1, GL_FALSE, (const GLdouble *)&proj);
	while ((!glfwWindowShouldClose(window)) && (!keyboard(window)))
	{
		//modelview = make_matrix(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display(pack->texture, pack, window);
		glUseProgram(pack->program);
		glUniformMatrix4dv(pack->model_id, 1, GL_FALSE,
			(const GLdouble *)&modelview);
		glBindVertexArray(pack->vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
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
	pack->program = glCreateProgram();
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pack->stats.points,
		(float*)pack->vertex, GL_STATIC_DRAW);
	ft_putendl("making vao");
	pack->vao = 0;
	glGenVertexArrays(1, &pack->vao);
	glBindBuffer(pack->vao, pack->vbo);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glewInit();
	glClearDepth((double)(INFINITY));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	ft_printf("program state: %d\n",
		make_program(pack, (ac > 2) ? av[2] : "herbe.jpg"));
	ft_printf("Renderer: %s\nOpenGL version supported %s\n",
		glGetString(GL_RENDERER), glGetString(GL_VERSION));
	return (main_loop(window, pack));
}
