/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:35:02 by snicolet          #+#    #+#             */
/*   Updated: 2018/11/18 14:22:19 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"
#include "tga.h"

static int			make_vao(const GLuint program, struct s_object *object)
{
	ft_putendl("making vbo");
	object->vbo = 0;
	glGenBuffers(1, &object->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)(sizeof(t_vertex_item) * object->stats.vertex),
		(float*)object->items,
		GL_STATIC_DRAW);
	ft_putendl("making faces indices");
	ft_opengl_buffer_load(&object->indices, GL_ELEMENT_ARRAY_BUFFER,
		object->faces, object->stats.faces * sizeof(t_v3i));
	ft_putendl("making vao");
	object->vao = 0;
	glGenVertexArrays(1, &object->vao);
	glBindVertexArray(object->vao);
	glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
	send_attributes(program, object);
	glBindVertexArray(0);
	return (0);
}

static int			make_texture(const GLuint program, struct s_texture_info *tex)
{
	static GLuint	texture_id = 1;
	GLint			id;

	ft_printf("loading texture %s\n\tprogram: %u\n\tuniform: %s\n",
		tex->filepath, program, tex->uniform);
	if (!tex->filepath)
		return (EXIT_FAILURE);
	glActiveTexture(GL_TEXTURE0 + texture_id);
	tex->id = SOIL_load_OGL_texture(tex->filepath, SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!tex->id)
	{
		ft_dprintf(STDERR_FILENO, "%s", "\terror on load\n");
		return (EXIT_FAILURE);
	}
	tex->opengl_id = (GLint)texture_id++;
	glBindTexture(GL_TEXTURE_2D, tex->id);
	id = glGetUniformLocation(program, tex->uniform);
	ft_printf("\tuniform id: %d\n\topengl id: %d\n", id, tex->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(id, tex->opengl_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (EXIT_SUCCESS);
}

static void			make_program_binds(t_vertex_pack *pack)
{
	glBindAttribLocation(pack->program, 0, "my_position");
	glBindAttribLocation(pack->program, 1, "my_color");
	glBindAttribLocation(pack->program, 2, "my_uv");
	glBindAttribLocation(pack->program, 3, "my_normal");
}

int					make_program(t_vertex_pack *pack)
{
	int						link_ok;

	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, SHADER_FRAGM)))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, SHADER_VERTEX)))
		return (2);
	ft_putendl("shaders ok");
	pack->program = glCreateProgram();
	if (!pack->program)
		return (3);
	make_program_binds(pack);
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	glGetProgramiv(pack->program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
		return (4);
	glUseProgram(pack->program);
	make_texture(pack->program, &pack->textures[DIFFUSE]);
	make_texture(pack->program, &pack->textures[NORMAL_MAP]);
	make_texture(pack->program, &pack->textures[AMBIANT_OCCLUSION]);
	make_vao(pack->program, &pack->object);
	ft_putendl("program done");
	return (0);
}
