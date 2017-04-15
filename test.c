/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 22:16:07 by snicolet          #+#    #+#             */
/*   Updated: 2017/04/15 16:05:54 by snicolet         ###   ########.fr       */
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

static void		display(GLFWwindow *window, GLuint vao, GLuint program)
{
	ft_putendl("entering display loop");
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	ft_putendl("exiting display loop");
}

static void		run_program(GLuint vs, GLuint fs, GLFWwindow *window,
	const float *points)
{
	GLuint			program;
	GLuint			vbo;
	GLuint			vao;

	ft_putendl("run program");
	//creation du programe
	program = glCreateProgram();
	glAttachShader(program, fs);
	glAttachShader(program, vs);
	glLinkProgram(program);
	ft_printf("program id: %u\n", program);

	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, points, GL_STATIC_DRAW);
	ft_printf("vbo: %u\n", vbo);

	//creation du vao
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	ft_printf("vao: %u\n", vao);
	display(window, vao, program);
}

static GLuint	compile_shader(GLuint type, const char *file)
{
	int			error_len;
	char		*error;
	GLuint		sh;
	char		*filecontent;
	GLint		success;

	filecontent = ft_readfile(file);
	if (!filecontent)
	{
		ft_dprintf(2, "error: failed to open file: %s\n", file);
		return (0);
	}
	sh = glCreateShader(type);
	glShaderSource(sh, 1, (const char **)(size_t)&filecontent, NULL);
	glCompileShader(sh);
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &error_len);
	free(filecontent);
	ft_putendl(file);
	if (error_len)
	{
		if ((error = malloc(sizeof(char) * ((unsigned)error_len + 1))))
		{
			glGetShaderInfoLog(sh, error_len, NULL, error);
			error[error_len] = '\0';
			ft_dprintf(2, "error returned: %s\n", error);
			free(error);
		}
		ft_dprintf(2, "error: failed to compile shader (err len: %d)\n", error_len);
		return (0);
	}
	ft_printf("shader compile stats: %i\n", success);
	return (sh);
}

static int		run_shaders(const float *points, GLFWwindow *window)
{
	GLuint			vs;
	GLuint			fs;

	ft_putendl("run shaders");
	//creation des shaders
	if (!(vs = compile_shader(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (0);
	if (!(fs = compile_shader(GL_VERTEX_SHADER, "vertex.glsl")))
		return (0);
	ft_printf("shaders:\n\tvs: %4u\n\tfs: %4u\n", vs, fs);
	run_program(vs, fs, window, points);
	ft_printf("cleaning shaders\n");
	return (0);
}

static int		main_loop(GLFWwindow *window)
{
	const float		points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	ft_putendl("run main loop");
	run_shaders(points, window);
	ft_putendl("exit main loop");
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (!(window = glfwCreateWindow(1280, 720, "test", NULL, NULL)))
	{
		glfwTerminate();
		return (2);
	}
	glfwMakeContextCurrent(window);
	ft_printf("renderer:       %s\n", glGetString(GL_RENDERER));
	ft_printf("opengl version: %s\n", glGetString(GL_VERSION));
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);
	glDepthFunc(GL_LESS);
	glClearDepth(0.0);
	glewInit();
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	return (main_loop(window));
}
