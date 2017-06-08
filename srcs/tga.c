/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 21:41:43 by snicolet          #+#    #+#             */
/*   Updated: 2017/06/08 23:49:21 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "tga.h"
#include <fcntl.h>
#include <stdlib.h>
#include <GL/glew.h>

static void			*load_tga_error(const char *error, char *file_content)
{
	if (file_content)
		free(file_content);
	ft_dprintf(2, "error: %s\n", error);
	return (NULL);
}

static unsigned int	*pixels_to_rgba(unsigned int *pixels, size_t n)
{
	unsigned int		color;
	unsigned char		rgba[4];

	while (n--)
	{
		color = pixels[n];
		rgba[2] = ((color & TGA_RED) >> 8);
		rgba[1] = ((color & TGA_GREEN) >> 16);
		rgba[0] = ((color & TGA_BLUE) >> 24);
		rgba[3] = (color & TGA_ALPHA);
		pixels[n] = *((unsigned int *)(size_t)rgba);
	}
	return (pixels);
}

/*
** load a tga file into a unsigned int * to retrive pixels,
** *specs will be filled by the function, in case of error each value will be
** set to 0 and NULL will be returned
*/

unsigned int		*load_tga(const char *filepath, t_tga *specs)
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
		return (pixels_to_rgba(pixels, specs->height * specs->width));
	}
	return (load_tga_error("failed to get pixels\n", file_content));
}

/*
** export pixels to a tga filepath, if the file already exists it will be
** replaced
** returns:
** 0 = everything was ok, no error
** 1 = wrong header type or memdup failed
** 2 = failed to open destination file, check permissions
*/

int					save_tga(const char *filepath, const t_tga *specs,
	const unsigned int *pixels)
{
	const size_t	size = specs->height * specs->width;
	int				fd;
	unsigned int	*px;
	size_t			p;

	if ((specs->type != TGA_TYPE_TC_RAW) ||
			(!(px = ft_memdup(pixels, size << 2))))
		return (1);
	if (!(fd = open(filepath, O_RDWR | O_TRUNC | O_CREAT, 0644)))
	{
		free(px);
		return (2);
	}
	p = size;
	while (p--)
		px[p] = (TGA_SR(px[p]) | TGA_SG(px[p]) | TGA_SB(px[p]) | TGA_SA(px[p]));
	write(fd, specs, TGA_SIZE);
	write(fd, px, size << 2);
	close(fd);
	free(px);
	return (0);
}

/*
** load fhe filepath tga file into the graphic card (must have a valid opengl
** context)
** return the id of the opengl texture, 0 in case of error
*/

GLuint				load_ogl_tga(const char *filepath)
{
	t_tga				header;
	GLuint				id;
	unsigned int		*pixels;

	if (!(pixels = load_tga(filepath, &header)))
		return (0);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, header.width, header.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
	return (id);
}
