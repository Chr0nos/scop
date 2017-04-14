/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 22:16:07 by snicolet          #+#    #+#             */
/*   Updated: 2017/04/14 21:44:51 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "draw.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void		error_handler(int id, const char *str)
{
	ft_dprintf(2, "[%2d] error: %s\n", id, str);
}

static void		display(GLFWwindow *window)
{
	(void)window;
}

static int		main_loop(GLFWwindow *window)
{
	const float		points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	char			*vertex_shader;
	char			*fragment_shader;
	GLuint			vbo;
	GLuint			program;
	GLuint			vs;
	GLuint			fs;

	//creation des shaders
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	//recuperation du code source des shaders
	fragment_shader = ft_readfile("fragment.glsl");
	vertex_shader = ft_readfile("vertex.glsl");
	if ((!fragment_shader) || (!vertex_shader))
	{
		ft_dprintf(2, "error while loading shaders\n");
		return (1);
	}

	//compilation des shaders
	glShaderSource(vs, 1, (const char **)(size_t)&vertex_shader, NULL);
	glCompileShader(vs);
	glShaderSource(fs, 1, (const char **)(size_t)&fragment_shader, NULL);
	glCompileShader(fs);
	ft_printf("shaders:\n\tvs: %4u\n\tfs: %4u\n", vs, fs);

	//creation du programe
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	ft_mfree(2, fragment_shader, vertex_shader);

	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, points, GL_STATIC_DRAW);	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		//glBindVertexArray(points);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		display(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return (0);
}

int				main(void)
{
	GLFWwindow		*window;

	if (!glfwInit())
		return (1);
	glfwSetErrorCallback(&error_handler);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!(window = glfwCreateWindow(1280, 720, "test", NULL, NULL)))
	{
		glfwTerminate();
		return (2);
	}
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glClearDepth(0.0);
	glewInit();
	return (main_loop(window));
}
