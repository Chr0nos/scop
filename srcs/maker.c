/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maker.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <snicolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/07 12:35:02 by snicolet          #+#    #+#             */
/*   Updated: 2017/05/07 16:55:59 by snicolet         ###   ########.fr       */
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

	ft_putendl("declaring UV opengl buffer");
	//les uv sont dans pack->uv (t_v2f *)
	pack->index_uv = 0;
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &pack->index_uv);
	glBindBuffer(GL_ARRAY_BUFFER, pack->index_uv);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(float) * pack->stats.uv),
		(float*)pack->uv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return (0);
}

int					make_program(t_vertex_pack *pack)
{
	ft_putendl("making program");
	if (!(pack->fs = ft_shader_compile(GL_FRAGMENT_SHADER, "fragment.glsl")))
		return (1);
	if (!(pack->vs = ft_shader_compile(GL_VERTEX_SHADER, "vertex.glsl")))
		return (2);
	ft_putendl("shaders ok");
	pack->program = glCreateProgram();
	glAttachShader(pack->program, pack->fs);
	glAttachShader(pack->program, pack->vs);
	glLinkProgram(pack->program);
	make_vao(pack);
	ft_putendl("program done");

	ft_printf("loading texture: %s\n", pack->texture_path);
	pack->texture = texture_load(pack->texture_path);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pack->texture);
	pack->texture_id = glGetUniformLocation(pack->program, "texture");
	glUniform1i(pack->texture_id, pack->texture);
	return (0);
}
