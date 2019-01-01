/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:33:10 by snicolet          #+#    #+#             */
/*   Updated: 2018/09/26 03:52:43 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static void			delete_textures(t_vertex_pack *pack)
{
	const GLuint	ids[TEXTURES_COUNT] = {
		pack->textures[DIFFUSE].id,
		pack->textures[NORMAL_MAP].id,
		pack->textures[AMBIANT_OCCLUSION].id
	};

	ft_putstr("deleting textures\n");
	glDeleteTextures(TEXTURES_COUNT, ids);
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
	glfwSetKeyCallback(window, &key_callback);
	glfwSetMouseButtonCallback(window, &mouse_button_callback);
	glfwSetCursorPosCallback(window, &mouse_pos_callback);
	glfwSetScrollCallback(window, &scroll_callback);
	glfwSetDropCallback(window, &dragndrop);
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

static int			command_parse(int ac, char **av,
	struct s_texture_info *textures)
{
	const char	*uniforms[] = {"texture_sampler", "normal_map", "ambiant_occlusion"};
	int			p;

	ft_bzero(textures, sizeof(struct s_texture_info) * TEXTURES_COUNT);
	p = 0;
	while ((p < ac) && (p < TEXTURES_COUNT))
	{
		ft_printf("requested texture: %s\n", av[p]);
		textures[p].filepath = av[p];
		textures[p].type = (enum e_texture_type)p;
		textures[p].uniform = uniforms[p];
		p++;
	}
	if (p != ac)
		ft_printf("%s", "warning: some parameters has been ignored.\n");
	return (EXIT_SUCCESS);
}

int					run_parse(int ac, char **av)
{
	const char		*filepath = av[0];
	int				ret;
	t_vertex_pack	*pack;
	double			parsing_started;

	ft_printf("running parsing of %s\n", filepath);
	pack = &g_pack;
	parsing_started = glfwGetTime();
	if (parse_obj(pack, filepath) != 0)
		return (21);
	ft_printf("parsing total time: %f secs\n", glfwGetTime() - parsing_started);
	ret = 2;
	if (pack->object.items)
	{
		command_parse(ac - 1, &av[1], pack->textures);
		ret = run_window(pack);
		ft_putendl("cleaning main structure pack");
		ft_mfree(2, pack->object.items, pack->object.faces);
	}
	ft_putendl("cleaning glfw");
	glfwTerminate();
	ft_printf("final return code: %d\n", ret);
	return (ret);
}
