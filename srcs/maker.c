/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:35:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/01 17:48:27 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ogl.h"
#include "opengl.h"

static int			make_vao(t_vertex_pack *pack)
{
	ft_putendl("making vbo");
	pack->vbo = 0;
	glGenBuffers(1, &pack->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(GLsizeiptr)(sizeof(t_vertex_item) * pack->stats.vertex),
		(float*)pack->items,
		GL_STATIC_DRAW);
	ft_putendl("making faces indices");
	ft_opengl_buffer_load(&pack->indices, GL_ELEMENT_ARRAY_BUFFER,
		pack->faces, pack->stats.faces * sizeof(t_v3i));
	ft_putendl("making vao");
	pack->vao = 0;
	glGenVertexArrays(1, &pack->vao);
	glBindVertexArray(pack->vao);
	glBindBuffer(GL_ARRAY_BUFFER, pack->vbo);
	send_attributes(pack);
	glBindVertexArray(0);
	return (0);
}

static GLint		make_texture(GLuint *image_id, const char *name,
		const char *filepath, const GLuint texture_id)
{
	t_vertex_pack		*pack;
	GLint				id;

	pack = get_pack(NULL);
	ft_printf("loading texture\n\tname: %s\n\tfrom: %s\n", name, filepath);
	glActiveTexture(GL_TEXTURE0 + texture_id);
	*image_id = SOIL_load_OGL_texture(filepath, SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!*image_id)
	{
		ft_putstr("\twarning: failed to load\n");
		return (-1);
	}
	glBindTexture(GL_TEXTURE_2D, *image_id);
	id = glGetUniformLocation(pack->program, name);
	ft_printf("\ttexture id: %d\n\timage id: %d\n\topengl id: %d\n", id,
			*image_id, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glUniform1i(id, (GLint)texture_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (id);
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
	int		link_ok;

	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, "vertex.glsl")))
		return (2);
	ft_putendl("shaders ok");
	pack->program = glCreateProgram();
	make_program_binds(pack);
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	glGetProgramiv(pack->program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
		return (3);
	glUseProgram(pack->program);
	pack->texture_id = make_texture(&pack->texture, "texture_sampler",
			pack->texture_path, 1);
	if (pack->normal_map_path)
		pack->normal_map_id = make_texture(&pack->normal_map, "normal_map",
				pack->normal_map_path, 2);
	make_vao(pack);
	ft_putendl("program done");
	return (0);
}
