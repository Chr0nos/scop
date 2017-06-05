/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 21:41:43 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/05 23:25:24 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <GL/glew.h>

#define TGA_TYPE_TC_RAW		2

#pragma pack(push, 1)

typedef struct		s_tga
{
	char			id;
	char			color_map;
	char			type;
	unsigned short	first_entry_index;
	unsigned short	color_map_len;
	unsigned char	bpp;
	unsigned short	x_offset;
	unsigned short	y_offset;
	unsigned short	width;
	unsigned short	height;
	unsigned char	depth;
	unsigned char	descriptor;
	char			drop[3];
}					t_tga;

#pragma pack(pop)

unsigned int	*load_tga(const char *filepath, t_tga *specs);
GLuint			load_OGL_tga(const char *filepath);

#define TGA_SIZE	sizeof(t_tga)

static void		*load_tga_error(const char *error, char *file_content)
{
	if (file_content)
		free(file_content);
	ft_dprintf(2, "error: %s\n", error);
	return (NULL);
}

unsigned int	*load_tga(const char *filepath, t_tga *specs)
{
	size_t			file_size;
	unsigned int	*pixels;
	char			*file_content;
	t_tga			*header;

	ft_bzero(specs, TGA_SIZE);
	if (!(file_content = ft_readfile(filepath, &file_size)))
		return load_tga_error("unable to read file\n", NULL);
	if (file_size <= TGA_SIZE)
		return (load_tga_error("invalid file or no content\n", file_content));
	header = (t_tga*)(size_t)file_content;
	if (header->type != TGA_TYPE_TC_RAW)
		return (load_tga_error("unsupported file format\n", file_content));
	if ((pixels = ft_memdup(&file_content[TGA_SIZE], file_size - TGA_SIZE)))
	{
		*specs = *header;
		free(file_content);
		return (pixels);
	}
	return (load_tga_error("failed to get pixels\n", file_content));
}

GLuint			load_OGL_tga(const char *filepath)
{
	t_tga				header;
	GLuint				id;
	unsigned int		*pixels;

	if (!(pixels = load_tga(filepath, &header)))
		return (0);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	return (id);
}

int				main(int ac, char **av)
{
	t_tga				header;
	unsigned int		*pixels;

	if ((ac > 1) && ((pixels = load_tga(av[1], &header))))
	{
		ft_printf("resolution %hux%hu\n", header.width, header.height);
		free(pixels);
	}
	return (0);
}
